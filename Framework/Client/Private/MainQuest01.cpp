#include "stdafx.h"
#include "MainQuest01.h"

#include "BTNode_Select.h"

#include "MainQuestNode01_01.h"

CMainQuest01::CMainQuest01()
{
}

HRESULT CMainQuest01::Initialize()
{
	__super::Initialize();

	/* �׷� ���⼭�� ���� �ϴ°� �ƴ϶� �׳� �迭�� ������ bool�� ó���� �ؾ��Ѵ�.*/

	m_pRootNode = CBTNode_Select::Create();
	
	CMainQuestNode01_01* m_pMainQuest01_01 = CMainQuestNode01_01::Create();

	m_pRootNode->Add_ChildNode(m_pMainQuest01_01);
	m_iQuestIndex += 1;

	m_pRootNode->Add_ChildNode(m_pMainQuest01_01);
	m_iQuestIndex += 1;

	m_pRootNode->Add_ChildNode(m_pMainQuest01_01);
	m_iQuestIndex += 1;

	return S_OK;
}

_bool CMainQuest01::Tick(const _float& fTimeDelta)
{
	// �̷��� �ϸ� �ȵǰڴµ�? ����
	m_pRootNode->Tick(fTimeDelta);

	return false;
}

void CMainQuest01::Free()
{
	__super::Free();

	Safe_Release(m_pRootNode);
}

