#include "stdafx.h"
#include "MainQuestNode_Invasion03.h"

#include "GameInstance.h"
#include "Utils.h"


#include "UI_Manager.h"
#include "UI_Fade.h"
#include "UI_PopupQuest.h"

#include "Game_Manager.h"
#include "Quest_Manager.h"

#include "TowerDefence_Manager.h"

CMainQuestNode_Invasion03::CMainQuestNode_Invasion03()
{
}

HRESULT CMainQuestNode_Invasion03::Initialize()
{
	__super::Initialize();

	m_fDeleteTime = 4.f;

	return S_OK;
}

void CMainQuestNode_Invasion03::Start()
{
	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	CTowerDefence_Manager::GetInstance()->Prepare_Defence();
}

CBTNode::NODE_STATE CMainQuestNode_Invasion03::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (CQuest_Manager::GetInstance()->Get_IsDefenceTalk())
	{
		m_fAccTime += fTimeDelta;

		if (m_fAccTime >= m_fDeleteTime)
		{
			CQuest_Manager::GetInstance()->Set_IsDefenceTalk(false);
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);
		}
	}

	if (!m_bIsFadeOut && CTowerDefence_Manager::GetInstance()->Get_CurrentPhase() == CTowerDefence_Manager::TOWER_DEFENCE_PHASE::DEFENCE_FINISH)
	{
		CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, 1.f);
		m_bIsFadeOut = true;
	}

	if (m_bIsFadeOut)
	{
		if (CUI_Manager::GetInstance()->Is_FadeFinished())
		{
			CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::QUESTEVENT_END);
			m_bIsClear = true;

			return NODE_STATE::NODE_FAIL;
		}
	}


	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Invasion03::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_Invasion03* CMainQuestNode_Invasion03::Create()
{
	CMainQuestNode_Invasion03* pInstance = new CMainQuestNode_Invasion03();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Invasion03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Invasion03::Free()
{
	__super::Free();
}
