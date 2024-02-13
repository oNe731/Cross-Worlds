#include "stdafx.h"
#include "SubQuestNode_Windmill04.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"
#include "Game_Manager.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "GameNpc.h"

CSubQuestNode_Windmill04::CSubQuestNode_Windmill04()
{
}

HRESULT CSubQuestNode_Windmill04::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("ǳ�� ����");
	m_strQuestContent = TEXT("���忡�� ����");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("ǳ�� ����");
	m_strNextQuestContent = TEXT("��� ���ĸ� ã��");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/02. SubQuest02_Verde_WindmillRepair/SubQuest_Windmill04.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CSubQuestNode_Windmill04::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	//m_pKuu = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Kuu"));
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pBeard = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, L"SwiftSolutionMaster");

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pBeard);

	/* ��ȭ ī�޶� ���� */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
		pActionCam->Start_Action_Talk(m_pBeard);

	/* ��ȭ */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CSubQuestNode_Windmill04::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

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
			CUI_PopupQuest::QUEST_INFO QuestDesc = {};
			QuestDesc.strType = m_strNextQuestTag;
			QuestDesc.strTitle = m_strNextQuestName;
			QuestDesc.strContents = m_strNextQuestContent;
			CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);

			m_bIsClear = true;
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MAIN_DIALOG);

			/* ��ȭ ī�޶� ���� */
			CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
			if (nullptr != pActionCam)
				pActionCam->Finish_Action_Talk();

			// �����Ͼ� ����.
			OBJECT_INIT_DESC pDesc = {};
			pDesc.vStartPosition = { -75.f, -7.f, -54.7f, 1.f };

			if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Prorotype_GameObject_Engineer_Npc"), &pDesc)))
			{
				MSG_BOX("Fail Clone : Quest Engineer Npc");
			}

			return NODE_STATE::NODE_FAIL;
		}

		m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
		m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

		CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

		TalkEvent();
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Windmill04::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_Windmill04::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("00_BeardSay_Hello.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBeard->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBeard->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("c31106000_p200_970011_std"));
		/* ��ȭ ī�޶� Ÿ�� ���� (ù ��ȭ ������ ������ NPC) */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("01_KuuSay_Hello.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU);
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_BeardSay_ohh.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBeard->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBeard->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("c31106000_p200_970010_std"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_LEFT);
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_KuuSay_Wath.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER); 
		break;
	case 4:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("04_BeardSay_HaHaHa.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBeard->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBeard->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("c31106000_p200_970003_std"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER); 
		break;
	case 5:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("05_KuuSay_EEEEEE.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_Idle03"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_RIGTH);
		break;
	case 6:
		// CSound_Manager::GetInstance()->Play_Sound(TEXT("06_KuuSay_NeverDont.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBeard->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBeard->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("c31106000_p200_970003_std"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 7:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("07_KuuSay_OK!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionPositive02"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER_FROM_BACK_NPC);
		break;
	}
}

CSubQuestNode_Windmill04* CSubQuestNode_Windmill04::Create()
{
	CSubQuestNode_Windmill04* pInstance = new CSubQuestNode_Windmill04();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Windmill04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Windmill04::Free()
{
	__super::Free();
}
