#include "stdafx.h"
#include "MainQuestNode_KingCall04.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

CMainQuestNode_KingCall04::CMainQuestNode_KingCall04()
{
}

HRESULT CMainQuestNode_KingCall04::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("���� �罽��");
	m_strQuestContent = TEXT("�罽���� ��ȭ�ϱ�");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("���� �罽��");
	m_strNextQuestContent = TEXT("�罽���� ��ȭ�ϱ�");

	return S_OK;
}

void CMainQuestNode_KingCall04::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

	m_pRuslan = GI->Find_GameObject(LEVELID::LEVEL_KINGDOMHALL, LAYER_NPC, TEXT("Ruslan"));
	Vec4 vSpotPos = Set_DestSpot(m_pRuslan);

	// �ӽ÷� monster�� 
	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));

	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strQuestTag;
	QuestDesc.strTitle = m_strQuestName;
	QuestDesc.strContents = m_strQuestContent;
	QuestDesc.bCreateSpot = true;
	QuestDesc.vDestPosition = vSpotPos;
	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
}

CBTNode::NODE_STATE CMainQuestNode_KingCall04::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_KINGDOMHALL)
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

void CMainQuestNode_KingCall04::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_KingCall04* CMainQuestNode_KingCall04::Create()
{
	CMainQuestNode_KingCall04* pInstance = new CMainQuestNode_KingCall04();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_KingCall04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_KingCall04::Free()
{
	__super::Free();
}
