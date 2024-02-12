#include "stdafx.h"
#include "SubQuestNode_Windmill07.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_Fade.h"
#include "UI_PopupQuest.h"
#include "Game_Manager.h"

#include "UIMinigame_Manager.h"
#include "Grandprix_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

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
	CUIMinigame_Manager::GetInstance()->Intro_Grandprix();
}

CBTNode::NODE_STATE CSubQuestNode_Windmill07::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;
	
	//if (!m_bIsStart)
	//{
	//	if (CUI_Manager::GetInstance()->Is_FadeFinished())
	//	{
	//		/* ��ȭ ī�޶� ���� */
	//		CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	//		if (nullptr != pActionCam)
	//			pActionCam->Finish_Action_Talk();
	//
	//		m_bIsStart = true;
	//	}
	//}

	// ���⼭ ���� ����.
	//if (KEY_TAP(KEY::N))
	//{
	//	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	//	QuestDesc.strType = m_strNextQuestTag;
	//	QuestDesc.strTitle = m_strNextQuestName;
	//	QuestDesc.strContents = m_strNextQuestContent;
	//	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
	//	m_bIsClear = true;
	//
	//	return NODE_STATE::NODE_FAIL;
	//}

	if (CGrandprix_Manager::GetInstance()->Is_GrandPrixEnd() && CUI_Manager::GetInstance()->Is_FadeFinished())
	{
		CUI_PopupQuest::QUEST_INFO QuestDesc = {};
		QuestDesc.strType = m_strNextQuestTag;
		QuestDesc.strTitle = m_strNextQuestName;
		QuestDesc.strContents = m_strNextQuestContent;
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
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
