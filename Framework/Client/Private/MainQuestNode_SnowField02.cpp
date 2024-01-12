#include "stdafx.h"
#include "MainQuestNode_SnowField02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CMainQuestNode_SnowField02::CMainQuestNode_SnowField02()
{
}

HRESULT CMainQuestNode_SnowField02::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("�ڿ���ũ ��������");
	m_strQuestContent = TEXT("���� ���� ���� �������� ���ϱ�");

	return S_OK;
}

void CMainQuestNode_SnowField02::Start()
{
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);
}

CBTNode::NODE_STATE CMainQuestNode_SnowField02::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_ICELAND)
	{
		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_SnowField02::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_SnowField02* CMainQuestNode_SnowField02::Create()
{
	CMainQuestNode_SnowField02* pInstance = new CMainQuestNode_SnowField02();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_SnowField02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_SnowField02::Free()
{
	__super::Free();
}
