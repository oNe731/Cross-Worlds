#include "stdafx.h"
#include "MainQuestNode_PlantKiller01.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

CMainQuestNode_PlantKiller01::CMainQuestNode_PlantKiller01()
{
}

HRESULT CMainQuestNode_PlantKiller01::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("������ ġ�� ���");
	m_strQuestContent = TEXT("�þȿ��� ����� �����");

	return S_OK;
}

void CMainQuestNode_PlantKiller01::Start()
{
	m_pCyan = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_NPC, TEXT("Cyan"));

	if (m_pCyan != nullptr)
	{
		Vec4 vSpotPos = Set_DestSpot(m_pCyan);

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

CBTNode::NODE_STATE CMainQuestNode_PlantKiller01::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_WITCHFOREST)
	{
		// �߸ŷ� �ٷ� �Ѿ�Դٸ�
		if (m_pCyan == nullptr)
		{
			m_pCyan = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_NPC, TEXT("Cyan"));

			if (m_pCyan != nullptr)
			{
				Vec4 vSpotPos = Set_DestSpot(m_pCyan);

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

		if (m_pQuestDestSpot != nullptr)
		{
			m_pQuestDestSpot->Tick(fTimeDelta);
			m_pQuestDestSpot->LateTick(fTimeDelta);

			if (m_pQuestDestSpot != nullptr)
			{
				if (m_pQuestDestSpot->Get_IsCol())
				{
					CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);

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

void CMainQuestNode_PlantKiller01::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_PlantKiller01* CMainQuestNode_PlantKiller01::Create()
{
	CMainQuestNode_PlantKiller01* pInstance = new CMainQuestNode_PlantKiller01();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_PlantKiller01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_PlantKiller01::Free()
{
	__super::Free();
}
