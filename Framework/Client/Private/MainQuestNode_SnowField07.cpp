#include "stdafx.h"
#include "MainQuestNode_SnowField07.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "Inventory_Manager.h"
#include "Game_Manager.h"
#include "Quest_Manager.h"
#include "Player.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"
#include "UI_Quest_Reward_Item.h"

CMainQuestNode_SnowField07::CMainQuestNode_SnowField07()
{
}

HRESULT CMainQuestNode_SnowField07::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("�屳 �轼���� �����ϱ�");
	m_strQuestContent = TEXT("�轼���� ��������");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/03.MainQuest_SnowField/MainQuest_SnowField07.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_SnowField07::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	//m_pKuu = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Kuu"));
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pJackson = GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, TEXT("GrimalKinML01"));

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pJackson);

	/* ��ȭ ī�޶� ���� */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
		pActionCam->Start_Action_Talk(m_pJackson);

	/* ��ȭ */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CMainQuestNode_SnowField07::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (!m_bIsRewarding)
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
				CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);
				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MAIN_DIALOG);

				/* ��ȭ ī�޶� ���� */
				CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
				if (nullptr != pActionCam)
					pActionCam->Finish_Action_Talk();

				/* ���⼭ ����Ʈ ���� �ޱ�.(����Ʈ ���� �� ������ return�ϱ�.*/
				CUI_Quest_Reward_Item::REWARDS_DESC ItemDesc = {};
				ItemDesc.bFirstSlot = true;
				ItemDesc.eFirstItem = CUI_Quest_Reward_Item::UI_QUESTREWARD_ITEM::REWARD_COIN;
				ItemDesc.iFirstAmount = 7777;

				ItemDesc.bSecondSlot = true;
				ItemDesc.eSecondItem = CUI_Quest_Reward_Item::UI_QUESTREWARD_ITEM::REWARD_HPPOTION;
				ItemDesc.iSecondAmount = 7;

				ItemDesc.bThirdSlot = true;
				ItemDesc.eThirdItem = CUI_Quest_Reward_Item::UI_QUESTREWARD_ITEM::REWARD_EXP;
				ItemDesc.iThirdAmount = 770;

				CUI_Manager::GetInstance()->Set_QuestRewards(&ItemDesc);
				CUI_Manager::GetInstance()->OnOff_QuestRewards(true, TEXT("���� ����"));

				CGame_Manager::GetInstance()->Get_Player()->Increase_Gold(ItemDesc.iFirstAmount);
				for (_int i = 0; i < ItemDesc.iSecondAmount; i++)
				{
					CInventory_Manager::GetInstance()->Prepare_Item(ITEM_TYPE::CONSUMPSION, ITEM_CODE::CONSUMPSION_HP);
				}
				CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Add_Exp(ItemDesc.iThirdAmount);

				m_bIsRewarding = true;
			}

			if (!m_bIsRewarding)
			{
				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

				TalkEvent();
			}
		}
	}

	else if (m_bIsRewarding)
	{
		if (CUI_Manager::GetInstance()->Is_QuestRewardWindowOff())
		{
			m_bIsClear = true;

			return NODE_STATE::NODE_SUCCESS;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_SnowField07::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_SnowField07::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_07_00_KuuSay_Hey~.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_07_01_JacksonSay_Thankyou.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC);
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_07_02_KuuSay_Pride.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionPositive02"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER_FROM_BACK_NPC);
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_07_01_JacksonSay_Thankyou.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		//m_pJackson->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		//m_pJackson->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand02Idle01"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC_FROM_BACK_KUU_AND_PLAYER);
		break;
	case 4:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_07_04_KuuSay_OK.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::ALL_RIGTH);
		break;
	}

}

CMainQuestNode_SnowField07* CMainQuestNode_SnowField07::Create()
{
	CMainQuestNode_SnowField07* pInstance = new CMainQuestNode_SnowField07();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_SnowField07");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_SnowField07::Free()
{
	__super::Free();
}
