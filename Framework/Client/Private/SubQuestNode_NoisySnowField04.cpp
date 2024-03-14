#include "stdafx.h"
#include "SubQuestNode_NoisySnowField04.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"

#include "Game_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "CurlingGame_Manager.h"

CSubQuestNode_NoisySnowField04::CSubQuestNode_NoisySnowField04()
{
}

HRESULT CSubQuestNode_NoisySnowField04::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("�ò����� �ڿ����� ����");
	m_strQuestContent = TEXT("�ò����� ���ڸ� ã�ƺ���");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("�ò����� �ڿ����� ����");
	m_strNextQuestContent = TEXT("��ῡ�� �¸��Ͽ� ������Ű��");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/04. SubQuest04_Vella_NoisySnowField/SubQuest_NoisySnowField04.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CSubQuestNode_NoisySnowField04::Start()
{
	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strNextQuestTag;
	QuestDesc.strTitle = m_strNextQuestName;
	QuestDesc.strContents = m_strNextQuestContent;
	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pNoisyMan = GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, L"Destroyer_Dummy");

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pNoisyMan);

	/* ��ȭ ī�޶� ���� */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
		pActionCam->Start_Action_Talk(m_pNoisyMan);

	/* ��ȭ */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CSubQuestNode_NoisySnowField04::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_ICELAND)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			Safe_Delete_Array(m_szpOwner);
			Safe_Delete_Array(m_szpTalk);

			m_iTalkIndex += 1;
			GI->Stop_Sound(CHANNELID::SOUND_UI);
			GI->Play_Sound(TEXT("UI_Fx_Comm_Btn_Dialogue_Page_1.mp3"), CHANNELID::SOUND_UI,
				GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

			if (m_iTalkIndex >= m_vecTalkDesc.size())
			{
				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MAIN_DIALOG);

				// �ִϸ��̼� ���
				m_pNoisyMan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Destroyer_Merge.ao|Destroyer_SkillBattleCry"));
				//CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_04_11.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);

				m_bIsGameReady = true;
			}

			if (m_iTalkIndex < m_vecTalkDesc.size())
			{
				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

				TalkEvent();
			}
		}

		if (m_bIsGameReady)
		{
			if (m_pNoisyMan->Get_Component_Model()->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_Destroyer_Merge.ao|Destroyer_SkillBattleCry") &&
				m_pNoisyMan->Get_Component_Model()->Get_CurrAnimationFrame() > 35)
			{
				// 1. ����� �����ִ� ī�޶� �׼��� �����Ѵ�.
				if (!m_bCameraAction)
				{
					m_bCameraAction = true;
					CCurlingGame_Manager::GetInstance()->Ready_Game();
				}
			}
		}

		// 2.����� �����ִ� ī�޶� �׼��� ����Ǿ��ٸ� ���� ���� �Ѿ��.
		CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
		if (nullptr != pActionCam)
		{
			if (m_bCameraAction && CCamera_Action::CAMERA_ACTION_TYPE::STADIUM != pActionCam->Get_Camera_ActionType())
			{
				m_bIsClear = true;

				return NODE_STATE::NODE_FAIL;
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_NoisySnowField04::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_NoisySnowField04::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		m_pNoisyMan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand01Idle01"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_04_01.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 2:
		m_pNoisyMan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand04Idle01"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_04_03.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSEndOfDonDonKing01"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU);
		break;
	case 4:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_04_04.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pNoisyMan->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_IDLE);
		m_pNoisyMan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Destroyer_Merge.ao|Destroyer_CSNeutralStand"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 5:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_04_05.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionAngry"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 6:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_04_06.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pNoisyMan->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pNoisyMan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Destroyer_Merge.ao|Destroyer_NeutralIdle01"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_RIGTH);
		break;
	case 7:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_04_07.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|MTG_Kuu_EmotionAnnoyed"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER_FROM_BACK_NPC);
		break;
	case 8:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_04_08.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pNoisyMan->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pNoisyMan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Destroyer_Merge.ao|Destroyer_CostumeMotion01"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;
	case 9:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_04_09.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 10:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("NoisyField_04_10.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pNoisyMan->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pNoisyMan->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Destroyer_Merge.ao|Destroyer_NeutralIdle03"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;
	}

}

CSubQuestNode_NoisySnowField04* CSubQuestNode_NoisySnowField04::Create()
{
	CSubQuestNode_NoisySnowField04* pInstance = new CSubQuestNode_NoisySnowField04();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_NoisySnowField04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_NoisySnowField04::Free()
{
	__super::Free();
}
