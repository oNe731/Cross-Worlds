#include "stdafx.h"
#include "MainQuestNode_Glanix07.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

CMainQuestNode_Glanix07::CMainQuestNode_Glanix07()
{
}

HRESULT CMainQuestNode_Glanix07::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("�ٽ� ����Ÿ�ٴϾƷ�");
	m_strQuestContent = TEXT("�Դ� ���� ���ư� ����Ÿ�ٴϾƷ� ���ư���");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("������");
	m_strNextQuestContent = TEXT("������ ���ư���");

	return S_OK;
}

void CMainQuestNode_Glanix07::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);
}

CBTNode::NODE_STATE CMainQuestNode_Glanix07::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
	{
		CUI_PopupQuest::QUEST_INFO QuestDesc = {};
		QuestDesc.strType = m_strNextQuestTag;
		QuestDesc.strTitle = m_strNextQuestName;
		QuestDesc.strContents = m_strNextQuestContent;
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);

		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Glanix07::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_Glanix07* CMainQuestNode_Glanix07::Create()
{
	CMainQuestNode_Glanix07* pInstance = new CMainQuestNode_Glanix07();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Glanix07");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Glanix07::Free()
{
	__super::Free();
}
