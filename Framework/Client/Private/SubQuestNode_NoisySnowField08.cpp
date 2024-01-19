#include "stdafx.h"
#include "SubQuestNode_NoisySnowField08.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "Game_Manager.h"

CSubQuestNode_NoisySnowField08::CSubQuestNode_NoisySnowField08()
{
}

HRESULT CSubQuestNode_NoisySnowField08::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("�ò����� �ڿ����� ����");
	m_strQuestContent = TEXT("���󿡰� ���ư���");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/02. SubQuest02_Verde_WindmillRepair/SubQuest_Windmill12.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CSubQuestNode_NoisySnowField08::Start()
{
	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pBella = GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, L"AquarisBella");

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pBella);

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

CBTNode::NODE_STATE CSubQuestNode_NoisySnowField08::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_ICELAND)
	{
		if (!m_bIsRewarding)
		{
			if (KEY_TAP(KEY::LBTN))
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_iTalkIndex += 1;

				if (m_iTalkIndex >= m_vecTalkDesc.size())
				{
					CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);
					CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 0);

					//CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
					//if (nullptr != pActionCam)
					//	pActionCam->Finish_Action_Talk();

					/* ���⼭ ����Ʈ ���� �ޱ�.(����Ʈ ���� �� ������ return�ϱ�.*/
					CUI_Manager::GetInstance()->OnOff_QuestRewards(true, TEXT("�ò����� �ڿ����� ����"));
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
				return NODE_STATE::NODE_FAIL;
			}
		}
	}



	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_NoisySnowField08::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_NoisySnowField08::TalkEvent()
{
	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("00_VerdeSay_hahaha.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBella->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBella->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Verde.ao|Verde_TurnL"));
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("01_KuuSay_Kiya.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		break;
	case 2:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("02_VerdeSay_Reward.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pBella->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pBella->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Verde.ao|Verde_TurnL"));
		break;
	case 3:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_KuuSay_Wahu!.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_Idle02"));
		break;
	}

}

CSubQuestNode_NoisySnowField08* CSubQuestNode_NoisySnowField08::Create()
{
	CSubQuestNode_NoisySnowField08* pInstance = new CSubQuestNode_NoisySnowField08();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_NoisySnowField08");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_NoisySnowField08::Free()
{
	__super::Free();
}
