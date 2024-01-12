#include "stdafx.h"
#include "MainQuestNode_IntroTour03.h"

#include "Quest_Manager.h"
#include "UI_Manager.h"
#include "GameInstance.h"
#include "Utils.h"

CMainQuestNode_IntroTour03::CMainQuestNode_IntroTour03()
{
}

HRESULT CMainQuestNode_IntroTour03::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("����Ÿ�ٴϾ� �ѷ�����");
	//m_strQuestContent = TEXT("����Ʈ 0 / 3 �Ϸ�");

	return S_OK;
}

void CMainQuestNode_IntroTour03::Start()
{
	//CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);
}

CBTNode::NODE_STATE CMainQuestNode_IntroTour03::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	m_strQuestContent = L"���� ����Ʈ " + to_wstring(CQuest_Manager::GetInstance()->Get_QuestClearStack());
	m_strQuestContent = m_strQuestContent + L" / 3 �Ϸ�";
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

	if (CQuest_Manager::GetInstance()->Get_QuestClearStack() >= 3)
	{
		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_IntroTour03::LateTick(const _float& fTimeDelta)
{
	if (m_pQuestDestSpot != nullptr)
		m_pQuestDestSpot->LateTick(fTimeDelta);
}

CMainQuestNode_IntroTour03* CMainQuestNode_IntroTour03::Create()
{
	CMainQuestNode_IntroTour03* pInstance = new CMainQuestNode_IntroTour03();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_IntroTour03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_IntroTour03::Free()
{
	__super::Free();
	Safe_Release(m_pQuestDestSpot);
}
