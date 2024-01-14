#include "stdafx.h"
#include "MainQuestNode_SnowField01.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CMainQuestNode_SnowField01::CMainQuestNode_SnowField01()
{
}

HRESULT CMainQuestNode_SnowField01::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("�ڿ���ũ ��������");
	m_strQuestContent = TEXT("�� ������ ������");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("�ڿ���ũ ��������");
	m_strNextQuestContent = TEXT("���� ���� ���� �������� ���ϱ�");

	return S_OK;
}

void CMainQuestNode_SnowField01::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);
}

CBTNode::NODE_STATE CMainQuestNode_SnowField01::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
	{
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_SnowField01::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_SnowField01* CMainQuestNode_SnowField01::Create()
{
	CMainQuestNode_SnowField01* pInstance = new CMainQuestNode_SnowField01();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_SnowField01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_SnowField01::Free()
{
	__super::Free();
}
