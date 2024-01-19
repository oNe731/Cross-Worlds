#include "stdafx.h"
#include "SubQuestNode_Wanted07.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CSubQuestNode_Wanted07::CSubQuestNode_Wanted07()
{
}

HRESULT CSubQuestNode_Wanted07::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("������ ���� ���");
	m_strQuestContent = TEXT("�༮���� ȥ������");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("������ ���� ���");
	m_strNextQuestContent = TEXT("���ٿ��� ���ư���");

	return S_OK;
}

void CSubQuestNode_Wanted07::Start()
{
}

CBTNode::NODE_STATE CSubQuestNode_Wanted07::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	/* ��񺴿��� �ΰ� */
	/* �ӽ� */
	if (KEY_TAP(KEY::N))
	{
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}


	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Wanted07::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_Wanted07* CSubQuestNode_Wanted07::Create()
{
	CSubQuestNode_Wanted07* pInstance = new CSubQuestNode_Wanted07();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Wanted07");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Wanted07::Free()
{
	__super::Free();
}
