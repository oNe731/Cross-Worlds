#include "stdafx.h"
#include "MainQuest01.h"

#include "BTNode_Select.h"

#include "MainQuestNode01_01.h"
#include "MainQuestNode01_02.h"
#include "MainQuestNode01_03.h"

CMainQuest01::CMainQuest01()
{
}

HRESULT CMainQuest01::Initialize()
{
	__super::Initialize();

	/* �׷� ���⼭�� ���� �ϴ°� �ƴ϶� �׳� �迭�� ������ bool�� ó���� �ؾ��Ѵ�.*/

	m_pRootNode = CBTNode_Select::Create();
	
	CMainQuestNode01_01* m_pMainQuest01_01 = CMainQuestNode01_01::Create();
	CMainQuestNode01_02* m_pMainQuest01_02 = CMainQuestNode01_02::Create();
	CMainQuestNode01_03* m_pMainQuest01_03 = CMainQuestNode01_03::Create();

	m_pRootNode->Add_ChildNode(m_pMainQuest01_01);
	m_pRootNode->Add_ChildNode(m_pMainQuest01_02);
	m_pRootNode->Add_ChildNode(m_pMainQuest01_03);

	return S_OK;
}

_bool CMainQuest01::Tick(const _float& fTimeDelta)
{
	if (m_pRootNode->Tick(fTimeDelta) == CBTNode::NODE_STATE::NODE_SUCCESS)
		return true;

	return false;
}

void CMainQuest01::LateTick(const _float& fTimeDelta)
{
}

CMainQuest01* CMainQuest01::Create()
{
	CMainQuest01* pInstance = new CMainQuest01();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuest01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuest01::Free()
{
	__super::Free();
}

