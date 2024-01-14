#include "stdafx.h"
#include "SubQuestNode_Wanted05.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CSubQuestNode_Wanted05::CSubQuestNode_Wanted05()
{
}

HRESULT CSubQuestNode_Wanted05::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("���� ���");
	m_strQuestContent = TEXT("�ձ��� ���ϳ��� ��ƶ�.");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("���ٿ��� ���ư���");
	m_strNextQuestContent = TEXT("���ٿ��� ��������");

	return S_OK;
}

void CSubQuestNode_Wanted05::Start()
{
	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestTag, m_strQuestTag, m_strQuestName, m_strQuestContent);

	/* ���� ����� ����� �� �ұ�? ��񺴿��� �ΰ�? */
}

CBTNode::NODE_STATE CSubQuestNode_Wanted05::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	/* ���� ����� ����� �� �ұ�? ��񺴿��� �ΰ�? */
	/* �ӽ� */
	if (KEY_TAP(KEY::N))
	{
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}


	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Wanted05::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_Wanted05* CSubQuestNode_Wanted05::Create()
{
	CSubQuestNode_Wanted05* pInstance = new CSubQuestNode_Wanted05();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Wanted05");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Wanted05::Free()
{
	__super::Free();
}
