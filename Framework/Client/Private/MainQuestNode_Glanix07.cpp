#include "stdafx.h"
#include "MainQuestNode_Glanix07.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CMainQuestNode_Glanix07::CMainQuestNode_Glanix07()
{
}

HRESULT CMainQuestNode_Glanix07::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("�ٽ� ����Ÿ����Ʒ�");
	m_strQuestContent = TEXT("�Դ� ���� ���ư� ����Ÿ����Ʒ� ���ư���");

	return S_OK;
}

void CMainQuestNode_Glanix07::Start()
{
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);
}

CBTNode::NODE_STATE CMainQuestNode_Glanix07::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
	{
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
