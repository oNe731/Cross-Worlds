#include "stdafx.h"
#include "SubQuestNode_Wanted05.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "UI_Manager.h"

CSubQuestNode_Wanted05::CSubQuestNode_Wanted05()
{
}

HRESULT CSubQuestNode_Wanted05::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("������ ���� ���");
	m_strQuestContent = TEXT("������ ���� �߰��ϱ�");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("������ ���� ���");
	m_strNextQuestContent = TEXT("�༮���� ȥ������");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/03. SubQuest03_Tumba_Wanted/SubQuest_Wanted05.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CSubQuestNode_Wanted05::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);

	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pCriminal = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, L"Criminal");

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pCriminal);

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

CBTNode::NODE_STATE CSubQuestNode_Wanted05::Tick(const _float& fTimeDelta)
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

			m_pCriminal->Set_Dead(true);

			return NODE_STATE::NODE_FAIL;
		}

		m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
		m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

		CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

		TalkEvent();
	}

	return NODE_STATE::NODE_RUNNING;

}

void CSubQuestNode_Wanted05::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_Wanted05::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("00_TumbaSay_Call.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand01Idle01"));
		break;
	case 1:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("01_KuuSay_Uh.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pCriminal->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pCriminal->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_BlackCircleAgent.ao|Fun"));
		break;
	case 2:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("02_TumbaSay_OkCallYou.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand04Idle01"));
		break;
	}
}

CSubQuestNode_Wanted05* CSubQuestNode_Wanted05::Create()
{
	CSubQuestNode_Wanted05* pInstance = new CSubQuestNode_Wanted05();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Wanted05");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Wanted05::Free()
{
	__super::Free();
}
