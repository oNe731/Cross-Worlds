#include "stdafx.h"
#include "MainQuestNode_FinalBattle07.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "Game_Manager.h"
#include "Character.h"
#include "Player.h"

#include "Quest_Manager.h"

#include "Camera_Group.h"

#include "Stellia.h"
#include "DreamMazeWitch_Npc.h"

CMainQuestNode_FinalBattle07::CMainQuestNode_FinalBattle07()
{
}

HRESULT CMainQuestNode_FinalBattle07::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("����� �Դ� ���� ����");
	m_strQuestContent = TEXT("���ڸ��� óġ�ϱ�");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/08.MainQuest_FinalBattle/MainQuest_FinalBattle07.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_FinalBattle07::Start()
{
	/* ī�޶� ���� ���� */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
	{
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
	}

	CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
	if (nullptr != pFollowCam)
	{
		/* ���� ���� �ʱ�ȭ */
		{
			pFollowCam->Reset_WideView_To_DefaultView(true);

			/* ī�޶� ������ ���� */
			pFollowCam->Set_TargetOffSet(Cam_Target_Offset_Stellia);
			pFollowCam->Set_LookAtOffSet(Cam_LookAt_Offset_Stellia);

			/* ���Ͻ� ���� */
			pFollowCam->Set_Distance(Cam_Dist_Follow_Stellia);

			pFollowCam->Set_Default_Position();

			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(true);
			CUI_Manager::GetInstance()->OnOff_GamePlaySetting(true);

			CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW);
		}

		///* ���� ���� */
		//{
		//	pFollowCam->Set_LockBoneNumber(3);
		//	
		//	CGameObject* pTarget = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_MONSTER, L"Stellia");
		//	if (nullptr != pTarget)
		//		pFollowCam->Start_LockOn(pTarget, Cam_Target_Offset_LockOn_Stellia, Cam_LookAt_Offset_LockOn_Stellia);
		//}

		/* ���̵�� ���ɼ� ���� */
		pFollowCam->Set_CanWideView(false);
	}

	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

	CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::QUESTEVENT_BOSS_KILL);

	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pStellia = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_MONSTER, TEXT("Stellia"));
	m_pWitch = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_NPC, TEXT("DreamMazeWitch"));

	if (m_pStellia == nullptr)
		MSG_BOX("FinalBattle07 : Fail Find Stellia");
	if (m_pWitch == nullptr)
		MSG_BOX("FinalBattle07 : Fail Find Witch");

	if (m_pStellia != nullptr && m_pWitch != nullptr)
	{
		m_pStellia->Get_Component_StateMachine()->Change_State(CStellia::STELLIA_COMBATIDLE);

		dynamic_cast<CDreamMazeWitch_Npc*>(m_pWitch)->Witch_BattleSet(m_pStellia);
		m_pWitch->Get_Component_StateMachine()->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_FOLLOWING);
	}

}

CBTNode::NODE_STATE CMainQuestNode_FinalBattle07::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (m_pStellia != nullptr)
	{
		if (m_pStellia->Is_ReserveDead() == false)
		{
			// ���� CombatIdle �� �ߵ�
			if (!m_bIsIntroTalk && m_pStellia->Get_Component_StateMachine()->Get_CurrState() == CStellia::STELLIA_COMBATIDLE)
			{
				m_bIsIntroTalk = true;
			}

			if (!m_bIsRage1Talk && m_pStellia->Get_Component_StateMachine()->Get_CurrState() == CStellia::STELLIA_RAGE1LOOP_TURN)
				m_bIsRage1Talk = true;

			if (!m_bIsRage2Talk && m_pStellia->Get_Component_StateMachine()->Get_CurrState() == CStellia::STELLIA_RAGE2LOOP)
				m_bIsRage2Talk = true;

			if (!m_bIsRage3Talk && m_pStellia->Get_Component_StateMachine()->Get_CurrState() == CStellia::STELLIA_RAGE3TURN_AROUND)
				m_bIsRage3Talk = true;

			if (m_bIsTalk)
			{
				m_fTime += fTimeDelta;

				if (m_fTime >= m_fTalkChangeTime)
				{
					CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::BATTLE_DIALOG);
					m_fTime = m_fTalkChangeTime - m_fTime;
					m_iTalkIndex += 1;
					m_bIsTalk = false;
				}
			}

			BossBattle_TalkEvent(fTimeDelta);
		}

		if (CQuest_Manager::GetInstance()->Get_IsBossKill())
		{
			CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);

			CQuest_Manager::GetInstance()->Set_CurQuestEvent(CQuest_Manager::QUESTEVENT_END);
			CQuest_Manager::GetInstance()->Set_IsBossKill(false);

			m_bIsClear = true;
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MAIN_DIALOG);

			dynamic_cast<CDreamMazeWitch_Npc*>(m_pWitch)->Set_IsFollowing(false);
			dynamic_cast<CDreamMazeWitch_Npc*>(m_pWitch)->Set_IsBattle(false);

			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_FinalBattle07::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_FinalBattle07::BossBattle_TalkEvent(const _float& fTimeDelta)
{
	switch (m_iTalkIndex)
	{
	case 0:
		if (m_bIsIntroTalk)
		{
			if (!m_bIsTalk)
			{
				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::BATTLE_DIALOG);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsTalk = true;
			}
		}
		break;

	case 1:
		if (m_bIsIntroTalk)
		{
			if (!m_bIsTalk)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::BATTLE_DIALOG);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsTalk = true;
			}
		}
		break;

	case 2:
		if (m_bIsRage1Talk)
		{
			if (!m_bIsTalk)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::BATTLE_DIALOG);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsTalk = true;
			}
		}
		break;

	case 3:
		if (m_bIsRage1Talk)
		{
			if (!m_bIsTalk)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::BATTLE_DIALOG);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsTalk = true;
			}
		}
		break;

	case 4:
		if (m_bIsRage1Talk)
		{
			if (!m_bIsTalk)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::BATTLE_DIALOG);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsTalk = true;
			}
		}
		break;

	case 5:
		if (m_bIsRage2Talk)
		{
			if (!m_bIsTalk)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::BATTLE_DIALOG);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsTalk = true;
			}
		}
		break;

	case 6:
		if (m_bIsRage2Talk)
		{
			if (!m_bIsTalk)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::BATTLE_DIALOG);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsTalk = true;
			}
		}
		break;

	case 7:
		if (m_bIsRage2Talk)
		{
			if (!m_bIsTalk)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::BATTLE_DIALOG);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsTalk = true;
			}
		}
		break;

	case 8:
		if (m_bIsRage3Talk)
		{
			if (!m_bIsTalk)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::BATTLE_DIALOG);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsTalk = true;
			}
		}
		break;

	case 9:
		if (m_bIsRage3Talk)
		{
			if (!m_bIsTalk)
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::BATTLE_DIALOG);
				CUI_Manager::GetInstance()->Set_BattleDialogue(m_szpTalk);

				TalkEvent();

				m_bIsTalk = true;
			}
		}
		break;
	}
}

void CMainQuestNode_FinalBattle07::TalkEvent()
{
	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("FinalBattle_07_00.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("FinalBattle_07_01.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("FinalBattle_07_02.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("FinalBattle_07_05.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("FinalBattle_07_07.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	case 9:
		break;
	}

}

CMainQuestNode_FinalBattle07* CMainQuestNode_FinalBattle07::Create()
{
	CMainQuestNode_FinalBattle07* pInstance = new CMainQuestNode_FinalBattle07();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_FinalBattle07");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_FinalBattle07::Free()
{
	__super::Free();
}
