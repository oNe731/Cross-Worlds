#include "stdafx.h"
#include "MainQuestNode_IntroTour02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Quest_Manager.h"
#include "UI_Manager.h"
#include "Sound_Manager.h"

#include "Camera_Group.h"

#include "Game_Manager.h"

CMainQuestNode_IntroTour02::CMainQuestNode_IntroTour02()
{
}

HRESULT CMainQuestNode_IntroTour02::Initialize()
{
	__super::Initialize();

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/01.MainQuest_IntroTour/MainQuest_IntroTour02.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	// string strLoad = Load.dump();

	return S_OK;
}

void CMainQuestNode_IntroTour02::Start()
{
	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	//m_pKuu = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Kuu"));
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());

	/* ��ȭ ī�޶� ���� */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
		pActionCam->Start_Action_Talk(nullptr);

	/* ��ȭ */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CMainQuestNode_IntroTour02::Tick(const _float& fTimeDelta)
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
			m_bIsClear = true;
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MAIN_DIALOG);
			
			CQuest_Manager::GetInstance()->Set_SubQuestRunning(CSubQuest::SUBQUEST_CHLOE_FINDCAT, true);
			CQuest_Manager::GetInstance()->Set_SubQuestRunning(CSubQuest::SUBQUEST_VERDE_WINDMILL, true);
			CQuest_Manager::GetInstance()->Set_SubQuestRunning(CSubQuest::SUBQUEST_VERDE_WANTED, true);

			/* �ƽ� ���� */
			CCamera_CutScene_Map* pCutSceneMap = dynamic_cast<CCamera_CutScene_Map*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE_MAP));
			if (nullptr != pCutSceneMap)
			{
				pCutSceneMap->Start_CutScene(LEVELID::LEVEL_EVERMORE, 0);
				pCutSceneMap->Reserve_NextCameraType(CAMERA_TYPE::FOLLOW);
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

void CMainQuestNode_IntroTour02::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_IntroTour02::TalkEvent()
{
	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
		// ��ȭ ���·� ���� �Ű��� �ִϸ��̼� �̸� ������. ���� ����.
		// Talk, Idle, Run, Walk. 
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("00_01_01KuuSay_Intro.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("00_01_02KuuSay_Hu.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("00_01_03KuuSay_HurryGo!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_talk01"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("00_01_04KuuSay_Happy!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_Idle02"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_Idle02"));
		break;
	}
}

CMainQuestNode_IntroTour02* CMainQuestNode_IntroTour02::Create()
{
	CMainQuestNode_IntroTour02* pInstance = new CMainQuestNode_IntroTour02();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_IntroTour02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_IntroTour02::Free()
{
	__super::Free();
}
