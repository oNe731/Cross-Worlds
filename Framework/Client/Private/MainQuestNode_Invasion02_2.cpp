#include "stdafx.h"
#include "MainQuestNode_Invasion02_2.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "Quest_Manager.h"

#include "GameNpc.h"
#include "UI_PopupQuest.h"

CMainQuestNode_Invasion02_2::CMainQuestNode_Invasion02_2()
{
}

HRESULT CMainQuestNode_Invasion02_2::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("�ı��� ����Ÿ�����");
	m_strQuestContent = TEXT("�� �� ���ư��� ��Ȳ�� ���캸��");

	return S_OK;
}

void CMainQuestNode_Invasion02_2::Start()
{
	Vec4 vSpotPos = { 0.f, 10.f, 71.5f, 1.f };
	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));

	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strQuestTag;
	QuestDesc.strTitle = m_strQuestName;
	QuestDesc.strContents = m_strQuestContent;
	QuestDesc.bCreateSpot = true;
	QuestDesc.vDestPosition = vSpotPos;
	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
}

CBTNode::NODE_STATE CMainQuestNode_Invasion02_2::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

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

void CMainQuestNode_Invasion02_2::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_Invasion02_2* CMainQuestNode_Invasion02_2::Create()
{
	CMainQuestNode_Invasion02_2* pInstance = new CMainQuestNode_Invasion02_2();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Invasion02_2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Invasion02_2::Free()
{
	__super::Free();
}
