#include "stdafx.h"
#include "SubQuestNode_NoisySnowField05.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "Game_Manager.h"

CSubQuestNode_NoisySnowField05::CSubQuestNode_NoisySnowField05()
{
}

HRESULT CSubQuestNode_NoisySnowField05::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("�ò����� �ڿ����� ����");
	m_strQuestContent = TEXT("��ῡ�� �¸��Ͽ� ������Ű��");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("�ò����� �ڿ����� ����");
	m_strNextQuestContent = TEXT("��ῡ�� �¸��Ͽ� ������Ű��");

	return S_OK;
}

void CSubQuestNode_NoisySnowField05::Start()
{
	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);
}

CBTNode::NODE_STATE CSubQuestNode_NoisySnowField05::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	// ���⼭ ���� ����.
	if (KEY_TAP(KEY::N))
	{
		m_bIsClear = true;

		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_NoisySnowField05::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_NoisySnowField05* CSubQuestNode_NoisySnowField05::Create()
{
	CSubQuestNode_NoisySnowField05* pInstance = new CSubQuestNode_NoisySnowField05();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_NoisySnowField05");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_NoisySnowField05::Free()
{
	__super::Free();
}
