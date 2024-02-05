#include "stdafx.h"
#include "SubQuestNode_Windmill07.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"
#include "Game_Manager.h"

CSubQuestNode_Windmill07::CSubQuestNode_Windmill07()
{
}

HRESULT CSubQuestNode_Windmill07::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("����� ��Ʋ!");
	m_strQuestContent = TEXT("����� ��Ʋ�� �ҳฦ ȥ������");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("����� ��Ʋ!");
	m_strNextQuestContent = TEXT("����� ��Ʋ�� �ҳฦ ȥ������");

	return S_OK;
}

void CSubQuestNode_Windmill07::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);
}

CBTNode::NODE_STATE CSubQuestNode_Windmill07::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	// ���⼭ ���� ����.
	if (KEY_TAP(KEY::N))
	{
		CUI_PopupQuest::QUEST_INFO QuestDesc = {};
		QuestDesc.strType = m_strNextQuestTag;
		QuestDesc.strTitle = m_strNextQuestName;
		QuestDesc.strContents = m_strNextQuestContent;
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
//		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);
		m_bIsClear = true;

		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Windmill07::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_Windmill07* CSubQuestNode_Windmill07::Create()
{
	CSubQuestNode_Windmill07* pInstance = new CSubQuestNode_Windmill07();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Windmill07");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Windmill07::Free()
{
	__super::Free();
}
