#include "stdafx.h"
#include "MainQuestNode_KingCall02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

#include "Game_Manager.h"

CMainQuestNode_KingCall02::CMainQuestNode_KingCall02()
{
}

HRESULT CMainQuestNode_KingCall02::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("�屳�� �ü�");
	m_strQuestContent = TEXT("������ ������ �ü��� ��������");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("������");
	m_strNextQuestContent = TEXT("������ ����");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/02.MainQuest_KingCall/MainQuest_KingCall02.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_KingCall02::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);

	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	//m_pKuu = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Kuu"));
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pSanson = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("GrimalKinML02"));

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pSanson);

	/* ī�޶� Ÿ�� ���� */
	// CGameObject* pTarget = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, L"Kuu");

	//if (nullptr != pTarget)
	//{

		// �ӽ� �ּ�
		//CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
		//if (nullptr != pActionCam)
		//{
		//	CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::ACTION);
		//	pActionCam->Start_Action_Talk(); //��� ȥ�ڸ� null
		//}


	//}

	/* ��ȭ */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CMainQuestNode_KingCall02::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (KEY_TAP(KEY::LBTN))
	{
		Safe_Delete_Array(m_szpOwner);
		Safe_Delete_Array(m_szpTalk);

		m_iTalkIndex += 1;

		if (m_iTalkIndex >= m_vecTalkDesc.size())
		{
			m_bIsClear = true;
			CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);
			CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 0);

			//CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
			//if (nullptr != pActionCam)
			//	pActionCam->Finish_Action_Talk();

			return NODE_STATE::NODE_FAIL;
		}

		m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
		m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

		CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

		TalkEvent();
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_KingCall02::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_KingCall02::TalkEvent()
{
	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("01_01_00_KuuSay_AngryHey!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionAngry"));
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("01_01_01_SansonSay_Hu....ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pSanson->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pSanson->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("stand01idle01"));
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("01_01_02_KuuSay_Hmm.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_CSSurprise02"));
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("01_01_03_SansonSay_Fact....ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pSanson->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pSanson->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("stand01idle01"));
		break;
	case 4:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("01_01_04_SansonSay_Hoxy.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pSanson->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pSanson->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("stand01idle01"));
		break;
	case 5:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("01_01_05_KuuSay_OK.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		break;
	case 6:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("01_01_06_SansonSay_Thankyou.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pSanson->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pSanson->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("stand01idle01"));
		break;
	}

}

CMainQuestNode_KingCall02* CMainQuestNode_KingCall02::Create()
{
	CMainQuestNode_KingCall02* pInstance = new CMainQuestNode_KingCall02();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_KingCall02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_KingCall02::Free()
{
	__super::Free();
}
