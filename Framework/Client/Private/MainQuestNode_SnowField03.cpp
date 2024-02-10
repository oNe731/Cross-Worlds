#include "stdafx.h"
#include "MainQuestNode_SnowField03.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"
#include "Game_Manager.h"

CMainQuestNode_SnowField03::CMainQuestNode_SnowField03()
{
}

HRESULT CMainQuestNode_SnowField03::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("�ֵ����� ���ְ� ã��");
	m_strQuestContent = TEXT("�ֵ������� ���ְ� ã�ƺ���");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("�ֵ����� ���ְ� ã��");
	m_strNextQuestContent = TEXT("�ֵ������� ���ְ� ã�ƺ���");


	return S_OK;
}

void CMainQuestNode_SnowField03::Start()
{
	m_pJackson = GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, TEXT("GrimalKinML01"));
	Vec4 vSpotPos = Set_DestSpot(m_pJackson);

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

CBTNode::NODE_STATE CMainQuestNode_SnowField03::Tick(const _float& fTimeDelta)
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

void CMainQuestNode_SnowField03::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_SnowField03* CMainQuestNode_SnowField03::Create()
{
	CMainQuestNode_SnowField03* pInstance = new CMainQuestNode_SnowField03();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_SnowField03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_SnowField03::Free()
{
	__super::Free();
}
