#include "stdafx.h"
#include "SubQuestNode_Wanted03.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

CSubQuestNode_Wanted03::CSubQuestNode_Wanted03()
{
}

HRESULT CSubQuestNode_Wanted03::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("������ ���� ���");
	m_strQuestContent = TEXT("������ ���̴� ���� ã��");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("������ ���� ���");
	m_strNextQuestContent = TEXT("������ ���� �߰��ϱ�");

	return S_OK;
}

void CSubQuestNode_Wanted03::Start()
{
	if (FAILED(GI->Add_GameObject(LEVEL_EVERMORE, _uint(LAYER_NPC), TEXT("Prorotype_GameObject_Criminal_Npc"), nullptr, &m_pCriminal)))
	{
		MSG_BOX("Fail AddGameObj : Quest Criminal_Npc");
	}
	else
	{
		Vec4 vSpotPos = { -38.75f, -6.75f, 39.5f, 1.f };
		// m_pCriminal->Get_Component<CTransform>(TEXT("Com_Transform"))->Set_State(CTransform::STATE_POSITION, { -40.f, -6.75f, 34.7f, 1.f });
		// m_pCriminal->Get_Component<CTransform>(TEXT("Com_Transform"))->FixRotation(0.f, -160.f, 0.f);

		// �ӽ÷� monster�� 
		m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));

		CUI_PopupQuest::QUEST_INFO QuestDesc = {};
		QuestDesc.strType = m_strQuestTag;
		QuestDesc.strTitle = m_strQuestName;
		QuestDesc.strContents = m_strQuestContent;
		QuestDesc.bCreateSpot = true;
		QuestDesc.vDestPosition = vSpotPos;
		CUI_Manager::GetInstance()->Set_QuestPopup(&QuestDesc);
	}
}

CBTNode::NODE_STATE CSubQuestNode_Wanted03::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	/* ���� ����� �߰��Ǹ� ����. */
	if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
	{
		if (m_pQuestDestSpot != nullptr)
		{
			m_pQuestDestSpot->Tick(fTimeDelta);
			m_pQuestDestSpot->LateTick(fTimeDelta);
	
			if (m_pQuestDestSpot != nullptr)
			{
				if (m_pQuestDestSpot->Get_IsCol())
				{
					CUI_PopupQuest::QUEST_INFO QuestDesc = {};
					QuestDesc.strType = m_strNextQuestTag;
					QuestDesc.strTitle = m_strNextQuestName;
					QuestDesc.strContents = m_strNextQuestContent;
					CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);

					m_bIsClear = true;
					m_pQuestDestSpot->Set_ReadyDelete(true);
					Safe_Release(m_pQuestDestSpot);
					return NODE_STATE::NODE_FAIL;
				}
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Wanted03::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_Wanted03* CSubQuestNode_Wanted03::Create()
{
	CSubQuestNode_Wanted03* pInstance = new CSubQuestNode_Wanted03();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Wanted03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Wanted03::Free()
{
	__super::Free();
}
