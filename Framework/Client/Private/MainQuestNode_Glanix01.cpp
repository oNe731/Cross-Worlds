#include "stdafx.h"
#include "MainQuestNode_Glanix01.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "UI_PopupQuest.h"
#include "Quest_Manager.h"

#include "GameNpc.h"

CMainQuestNode_Glanix01::CMainQuestNode_Glanix01()
{
}

HRESULT CMainQuestNode_Glanix01::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("�� ����� �����ϱ�");
	m_strQuestContent = TEXT("�ڿ���ũ ���� ���� �� ���� ������");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("�� ����� �����ϱ�");
	m_strNextQuestContent = TEXT("�ڿ���ũ ���� ���� �� ���� ������");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/04.MainQuest_Glanix/MainQuest_Glanix01.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_Glanix01::Start()
{
	CQuest_Manager::GetInstance()->Set_SubQuestRunning(CSubQuest::SUBQUEST_VELLA_NOISYSNOWFIELD, true);

	m_pAren = GI->Find_GameObject(LEVELID::LEVEL_ICELAND, LAYER_NPC, TEXT("Aren"));
	if (m_pAren != nullptr)
	{
		Vec4 vSpotPos = Set_DestSpot(m_pAren);

		m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));
	
		CUI_PopupQuest::QUEST_INFO QuestDesc = {};
		QuestDesc.strType = m_strQuestTag;
		QuestDesc.strTitle = m_strQuestName;
		QuestDesc.strContents = m_strQuestContent;
		QuestDesc.bCreateSpot = true;
		QuestDesc.vDestPosition = vSpotPos;
		CUI_Manager::GetInstance()->Set_QuestPopup(&QuestDesc);
	}

	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
}

CBTNode::NODE_STATE CMainQuestNode_Glanix01::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_ICELAND)
	{
		if (!Is_EndCameraBlender())
			return NODE_STATE::NODE_RUNNING;

		if (!m_bIsStart)
		{
			/* ��ȭ */
			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::MINI_DIALOG);
			CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();

			m_bIsStart = true;
		}

		m_fTime += fTimeDelta;

		if (m_fTime >= 3.f)
		{
			if (m_iTalkIndex < m_vecTalkDesc.size())
			{
				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_iTalkIndex += 1;
				GI->Stop_Sound(CHANNELID::SOUND_UI);
				GI->Play_Sound(TEXT("UI_Fx_Comm_Dialog_Text_1.mp3"), CHANNELID::SOUND_UI,
					GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

				if (m_iTalkIndex >= m_vecTalkDesc.size())
					CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

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

		if (m_pQuestDestSpot != nullptr)
		{
			m_pQuestDestSpot->Tick(fTimeDelta);
			m_pQuestDestSpot->LateTick(fTimeDelta);

			if (m_pQuestDestSpot != nullptr)
			{
				if (m_pQuestDestSpot->Get_IsCol())
				{
					CUI_PopupQuest::QUEST_INFO QuestDesc = {};
					QuestDesc.strType = m_strNextQuestTag;
					QuestDesc.strTitle = m_strNextQuestName;
					QuestDesc.strContents = m_strNextQuestContent;
					CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);

					m_bIsClear = true;
					m_pQuestDestSpot->Set_ReadyDelete(true);
					Safe_Release(m_pQuestDestSpot);

					return NODE_STATE::NODE_FAIL;
				}
			}
		}

		// �׽�Ʈ�� Ű
		if (KEY_TAP(KEY::M))
		{
			m_bIsClear = true;

			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Glanix01::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_Glanix01::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_01_00_KuuSay_Hmm....ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("03_01_01_KuuSay_HHH.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK, TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_EmotionDepressed"));
		break;
	}
}

CMainQuestNode_Glanix01* CMainQuestNode_Glanix01::Create()
{
	CMainQuestNode_Glanix01* pInstance = new CMainQuestNode_Glanix01();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Glanix01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Glanix01::Free()
{
	__super::Free();
}
