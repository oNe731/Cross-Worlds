#include "stdafx.h"
#include "MainQuestNode_Glanix08.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CMainQuestNode_Glanix08::CMainQuestNode_Glanix08()
{
}

HRESULT CMainQuestNode_Glanix08::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("������");
	m_strQuestContent = TEXT("������ ����");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("�罽������ �����ϱ�");
	m_strNextQuestContent = TEXT("�罽���� ��ȭ�ϱ�");

	return S_OK;
}

void CMainQuestNode_Glanix08::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);
}

CBTNode::NODE_STATE CMainQuestNode_Glanix08::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_KINGDOMHALL)
	{
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Glanix08::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_Glanix08::TalkEvent()
{
}

CMainQuestNode_Glanix08* CMainQuestNode_Glanix08::Create()
{
	CMainQuestNode_Glanix08* pInstance = new CMainQuestNode_Glanix08();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Glanix08");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Glanix08::Free()
{
	__super::Free();
}
