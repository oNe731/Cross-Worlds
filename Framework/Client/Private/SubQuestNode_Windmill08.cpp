#include "stdafx.h"
#include "SubQuestNode_Windmill08.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_Fade.h"
#include "UI_PopupQuest.h"
#include "Game_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "Character_Manager.h"
#include "Player.h"

CSubQuestNode_Windmill08::CSubQuestNode_Windmill08()
{
}

HRESULT CSubQuestNode_Windmill08::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("����� ��Ʋ!");
	m_strQuestContent = TEXT("����� ��Ʋ�� �ҳฦ ȥ������");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("ǳ�� ����");
	m_strNextQuestContent = TEXT("�����𿡰� ���ư���");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/02. SubQuest02_Verde_WindmillRepair/SubQuest_Windmill08.json");

	for (const auto& talkDesc : Load)
	{
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CSubQuestNode_Windmill08::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pEngineer = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, L"Engineer_Dummy");

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pEngineer);

	/* ��ȭ ī�޶� ���� */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
		pActionCam->Start_Action_Talk(m_pEngineer);

	/* ��ȭ */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CSubQuestNode_Windmill08::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (KEY_TAP(KEY::LBTN) && !m_bIsFadeOut)
	{
		Safe_Delete_Array(m_szpOwner);
		Safe_Delete_Array(m_szpTalk);

		m_iTalkIndex += 1;
		GI->Stop_Sound(CHANNELID::SOUND_UI);
		GI->Play_Sound(TEXT("UI_Fx_Comm_Btn_Dialogue_Page_1.mp3"), CHANNELID::SOUND_UI,
			GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

		if (m_iTalkIndex >= m_vecTalkDesc.size())
		{
			// ���� ��忡�� ����
			CUI_PopupQuest::QUEST_INFO QuestDesc = {};
			QuestDesc.strType = m_strNextQuestTag;
			QuestDesc.strTitle = m_strNextQuestName;
			QuestDesc.strContents = m_strNextQuestContent;
			CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);

			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MAIN_DIALOG);
			m_bIsFadeOut = true;
		}

		if (m_iTalkIndex < m_vecTalkDesc.size())
		{
			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();
		}
	}

	if (!m_bIsFadeIn && m_bIsFadeOut)
	{
		CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, 1.f);

		m_bIsFadeIn = true;
	}

	if (m_bIsFadeIn)
	{
		if (CUI_Manager::GetInstance()->Is_FadeFinished())
		{
			//
			CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 1.f);
			CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
			if (nullptr != pFollowCam)
			{
				pFollowCam->Reset_WideView_To_DefaultView(true);
				pFollowCam->Set_Default_Position();
				CCamera_Manager::GetInstance()->Set_CurCamera(pFollowCam->Get_Key());

			}

			/* Ui�� �÷��̾� ��ǲ�� �����ش�. */
			CUI_Manager::GetInstance()->OnOff_GamePlaySetting(true);
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(true);
			//

			m_bIsClear = true;
			m_pEngineer->Set_Dead(true);

			CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::ENGINEER)->Set_Useable(true);

			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Windmill08::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_Windmill08::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("SQ_02_08_00_KuuSay_Happy.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_Idle02"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("SQ_02_08_01_EGSay_Sad.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pEngineer->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pEngineer->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Engineer_SoulDiver.ao|Engineer_CostumeMotion01"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Windmill_08_02.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSHatchOut01"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Windmill_08_03.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pEngineer->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pEngineer->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Engineer_SoulDiver.ao|Engineer_NeutralIdle01"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_RIGTH);
		break;
	case 4:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Windmill_08_04.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER_FROM_BACK_NPC);
		break;
	case 5:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Windmill_08_05.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pEngineer->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pEngineer->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Engineer_SoulDiver.ao|Engineer_ControlViewPoint02"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;
	case 6:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Windmill_08_06.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSHereComesNewCrisis01E"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU);
		break;
	case 7:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Windmill_08_07.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSHereComesNewCrisis01D"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER_FROM_BACK_NPC);
		break;
	case 8:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("SQ_02_08_09_EGSay_Go!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pEngineer->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pEngineer->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Engineer_SoulDiver.ao|Engineer_ClassAction01"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;
	}
}

CSubQuestNode_Windmill08* CSubQuestNode_Windmill08::Create()
{
	CSubQuestNode_Windmill08* pInstance = new CSubQuestNode_Windmill08();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Windmill08");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Windmill08::Free()
{
	__super::Free();
}
