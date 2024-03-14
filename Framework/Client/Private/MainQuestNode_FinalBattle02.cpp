#include "stdafx.h"
#include "MainQuestNode_FinalBattle02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

CMainQuestNode_FinalBattle02::CMainQuestNode_FinalBattle02()
{
}

HRESULT CMainQuestNode_FinalBattle02::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("������");
	m_strQuestContent = TEXT("�Ǹ��� ����� ��ȭ����");

	return S_OK;
}

void CMainQuestNode_FinalBattle02::Start()
{
	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strQuestTag;
	QuestDesc.strTitle = m_strQuestName;
	QuestDesc.strContents = m_strQuestContent;
	CUI_Manager::GetInstance()->Set_QuestPopup(&QuestDesc);

	if (FAILED(GI->Add_GameObject(LEVEL_WITCHFOREST, _uint(LAYER_NPC), TEXT("Prorotype_GameObject_DreamMazeWitch_Npc"), nullptr, &m_pWitch)))
	{
		MSG_BOX("Fail AddGameObj : Quest DreamMazeWitch_Npc");
	}
	else
	{
		m_pWitch->Get_Component_Transform()->Set_State(CTransform::STATE_POSITION, Vec4(166.8f, 3.f, 165.5f, 1.f));
		m_pWitch->Get_Component_Transform()->FixRotation(0.f, -162.f, 0.f);
		
		Vec4 vSpotPos = { 157.7f, 0.f, 134.3f, 1.f };

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

CBTNode::NODE_STATE CMainQuestNode_FinalBattle02::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_WITCHFOREST)
	{
		if (m_pQuestDestSpot != nullptr)
		{
			m_pQuestDestSpot->Tick(fTimeDelta);
			m_pQuestDestSpot->LateTick(fTimeDelta);

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

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_FinalBattle02::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_FinalBattle02* CMainQuestNode_FinalBattle02::Create()
{
	CMainQuestNode_FinalBattle02* pInstance = new CMainQuestNode_FinalBattle02();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_FinalBattle02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_FinalBattle02::Free()
{
	__super::Free();
}
