#include "stdafx.h"
#include "MainQuestNode_SnowField02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "UI_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

CMainQuestNode_SnowField02::CMainQuestNode_SnowField02()
{
}

HRESULT CMainQuestNode_SnowField02::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[메인]");
	m_strQuestName = TEXT("코에루크 설원으로");
	m_strQuestContent = TEXT("동쪽 문을 통해 설원으로 향하기");

	m_strNextQuestTag = TEXT("[메인]");
	m_strNextQuestName = TEXT("코에루크 설원으로");
	m_strNextQuestContent = TEXT("동쪽 문을 통해 설원으로 향하기");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/03.MainQuest_SnowField/MainQuest_SnowField02.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_SnowField02::Start()
{
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());

	/* 대화 */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->OnOff_DialogWindow(true, 1);
	CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

	/* 대화 카메라 세팅 */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
		pActionCam->Start_Action_Talk(nullptr);

	TalkEvent();
}

CBTNode::NODE_STATE CMainQuestNode_SnowField02::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	m_fTime += fTimeDelta;

	if (m_fTime >= 3.f)
	{
		if (m_iTalkIndex < m_vecTalkDesc.size())
		{
			Safe_Delete_Array(m_szpOwner);
			Safe_Delete_Array(m_szpTalk);

			m_iTalkIndex += 1;

			if (m_iTalkIndex >= m_vecTalkDesc.size())
				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);

			if (m_iTalkIndex < m_vecTalkDesc.size())
			{
				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

				TalkEvent();
			}

			m_fTime = m_fTalkChangeTime - m_fTime;
		}
	}


	if (GI->Get_CurrentLevel() == LEVEL_ICELAND)
	{
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

		m_bIsClear = true;

		/* 대화 카메라 종료 */
		CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
		if (nullptr != pActionCam)
			pActionCam->Finish_Action_Talk();

		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_SnowField02::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_SnowField02::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_02_00_KuuSay_AhHa!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_02_01_KuuSay_Heong!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	}
}

CMainQuestNode_SnowField02* CMainQuestNode_SnowField02::Create()
{
	CMainQuestNode_SnowField02* pInstance = new CMainQuestNode_SnowField02();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_SnowField02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_SnowField02::Free()
{
	__super::Free();
}
