#include "stdafx.h"
#include "SubQuestNode_NoisySnowField07.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CSubQuestNode_NoisySnowField07::CSubQuestNode_NoisySnowField07()
{
}

HRESULT CSubQuestNode_NoisySnowField07::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("�ò����� �ڿ����� ����");
	m_strQuestContent = TEXT("���󿡰� ���ư���");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("�ò����� �ڿ����� ����");
	m_strNextQuestContent = TEXT("���󿡰� ���ư���");


	return S_OK;
}

void CSubQuestNode_NoisySnowField07::Start()
{
	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

	m_pBella = GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, TEXT("AquarisBella"));
	Vec4 vSpotPos = Set_DestSpot(m_pBella);

	// �ӽ÷� monster�� 
	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));
}

CBTNode::NODE_STATE CSubQuestNode_NoisySnowField07::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_ICELAND)
	{
		if (m_pQuestDestSpot != nullptr)
		{
			m_pQuestDestSpot->Tick(fTimeDelta);
			m_pQuestDestSpot->LateTick(fTimeDelta);

			if (m_pQuestDestSpot != nullptr)
			{
				if (m_pQuestDestSpot->Get_IsCol())
				{
					CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);
					m_pQuestDestSpot->Set_ReadyDelete(true);
					m_bIsClear = true;
					Safe_Release(m_pQuestDestSpot);

					return NODE_STATE::NODE_FAIL;
				}
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_NoisySnowField07::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_NoisySnowField07* CSubQuestNode_NoisySnowField07::Create()
{
	CSubQuestNode_NoisySnowField07* pInstance = new CSubQuestNode_NoisySnowField07();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_NoisySnowField07");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_NoisySnowField07::Free()
{
	__super::Free();
}
