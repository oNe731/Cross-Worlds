#include "stdafx.h"
#include "SubQuestNode_NoisySnowField03.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

CSubQuestNode_NoisySnowField03::CSubQuestNode_NoisySnowField03()
{
}

HRESULT CSubQuestNode_NoisySnowField03::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("�ò����� �ڿ����� ����");
	m_strQuestContent = TEXT("�ò����� ���ڸ� ã�ƺ���");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("�ò����� �ڿ����� ����");
	m_strNextQuestContent = TEXT("�ò����� ���ڸ� ã�ƺ���");

	return S_OK;
}

void CSubQuestNode_NoisySnowField03::Start()
{
	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strQuestTag;
	QuestDesc.strTitle = m_strQuestName;
	QuestDesc.strContents = m_strQuestContent;
	CUI_Manager::GetInstance()->Set_QuestPopup(&QuestDesc);

	m_pNoisyMan = GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, TEXT("Destroyer_Dummy"));

	// �ӽ÷� monster�� 
	if (m_pNoisyMan != nullptr)
	{
		Vec4 vSpotPos = Set_DestSpot(m_pNoisyMan);
		m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));
	
		CUI_PopupQuest::QUEST_INFO QuestDesc = {};
		QuestDesc.strType = m_strQuestTag;
		QuestDesc.strTitle = m_strQuestName;
		QuestDesc.strContents = m_strQuestContent;
		QuestDesc.bCreateSpot = true;
		QuestDesc.vDestPosition = vSpotPos;
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
	}
}

CBTNode::NODE_STATE CSubQuestNode_NoisySnowField03::Tick(const _float& fTimeDelta)
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

void CSubQuestNode_NoisySnowField03::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_NoisySnowField03* CSubQuestNode_NoisySnowField03::Create()
{
	CSubQuestNode_NoisySnowField03* pInstance = new CSubQuestNode_NoisySnowField03();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_NoisySnowField03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_NoisySnowField03::Free()
{
	__super::Free();
}
