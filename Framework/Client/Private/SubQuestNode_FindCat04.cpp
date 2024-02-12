#include "stdafx.h"
#include "SubQuestNode_FindCat04.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"
#include "Quest_Manager.h"

#include "Animals.h"

CSubQuestNode_FindCat04::CSubQuestNode_FindCat04()
{
}

HRESULT CSubQuestNode_FindCat04::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("������ ã��");
	m_strQuestContent = TEXT("�����̸� ã������");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("������ ã��");
	m_strNextQuestContent = TEXT("�����̸� ã������");

	return S_OK;
}

void CSubQuestNode_FindCat04::Start()
{
	CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::QUESTEVENT_FIND_ANIMAL);

	// ������ �� ���� ����.
	if (m_pDog == nullptr)
	{
		OBJECT_INIT_DESC DogDesc = {};
		DogDesc.vStartPosition = { -86.8f, 0.f, 134.6f, 1.f };

		if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, _uint(LAYER_DYNAMIC), TEXT("Prorotype_GameObject_Animal_WelshCorgi"), &DogDesc, &m_pDog)))
			MSG_BOX("Retry Add_GameObject Fail : m_pDog");
	}

	m_pChloe = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Chloe"));
	Vec4 vSpotPos = Set_DestSpot(m_pChloe);

	// �ӽ÷� monster�� 
	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));

	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strQuestTag;
	QuestDesc.strTitle = m_strQuestName;
	QuestDesc.strContents = m_strQuestContent;
	QuestDesc.bCreateSpot = true;
	QuestDesc.vDestPosition = vSpotPos;
	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName , &QuestDesc);
}

CBTNode::NODE_STATE CSubQuestNode_FindCat04::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
	{
		// �����̰� start���� ������ �� �ƴٸ� ����.
		if (m_pDog == nullptr)
		{
			OBJECT_INIT_DESC DogDesc = {};
			DogDesc.vStartPosition = { -86.8f, 0.f, 134.6f, 1.f };

			if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, _uint(LAYER_DYNAMIC), TEXT("Prorotype_GameObject_Animal_WelshCorgi"), &DogDesc, &m_pDog)))
				MSG_BOX("Retry Add_GameObject Fail : m_pDog");
		}

		if (m_pQuestDestSpot != nullptr)
		{
			if (dynamic_cast<CAnimals*>(m_pDog)->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Get_CurrState() == CAnimals::STATE::STATE_LIFT)
			{
				m_pQuestDestSpot->Tick(fTimeDelta);
				m_pQuestDestSpot->LateTick(fTimeDelta);

				if (m_pQuestDestSpot != nullptr)
				{
					if (m_pQuestDestSpot->Get_IsCol())
					{

						// ������ ���� ����.
						Vec3 vDogScale = m_pDog->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_Scale();
						Matrix matChloeWorld = m_pChloe->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_WorldMatrix();
						m_pDog->Get_Component<CTransform>(TEXT("Com_Transform"))->Set_WorldMatrix(matChloeWorld);
						m_pDog->Get_Component<CTransform>(TEXT("Com_Transform"))->Set_Scale(vDogScale);

						// ������ ��ġ ����.
						Vec4 vChloePos = m_pChloe->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_Position();
						vChloePos.x -= 1.f;
						m_pDog->Get_Component<CTransform>(TEXT("Com_Transform"))->Set_State(CTransform::STATE_POSITION, vChloePos);

						CUI_PopupQuest::QUEST_INFO QuestDesc = {};
						QuestDesc.strType = m_strNextQuestTag;
						QuestDesc.strTitle = m_strNextQuestName;
						QuestDesc.strContents = m_strNextQuestContent;
						CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);

						dynamic_cast<CAnimals*>(m_pDog)->Set_Lift(false);
						dynamic_cast<CAnimals*>(m_pDog)->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CAnimals::STATE::STATE_STAND);

						m_bIsClear = true;
						m_pQuestDestSpot->Set_ReadyDelete(true);
						Safe_Release(m_pQuestDestSpot);
						return NODE_STATE::NODE_FAIL;
					}
				}
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_FindCat04::LateTick(const _float& fTimeDelta)
{
	if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
	{
		if (m_pQuestDestSpot != nullptr)
			m_pQuestDestSpot->LateTick(fTimeDelta);
	}
}

CSubQuestNode_FindCat04* CSubQuestNode_FindCat04::Create()
{
	CSubQuestNode_FindCat04* pInstance = new CSubQuestNode_FindCat04();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_FindCat04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_FindCat04::Free()
{
	__super::Free();
}
