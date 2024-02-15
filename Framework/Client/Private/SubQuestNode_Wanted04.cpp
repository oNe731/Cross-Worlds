#include "stdafx.h"
#include "SubQuestNode_Wanted04.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"
#include "Game_Manager.h"

#include "Criminal_Npc.h"

CSubQuestNode_Wanted04::CSubQuestNode_Wanted04()
{
}

HRESULT CSubQuestNode_Wanted04::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("������ ���� ���");
	m_strQuestContent = TEXT("������ ���� �߰��ϱ�");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("������ ���� ���");
	m_strNextQuestContent = TEXT("������ ���� �߰��ϱ�");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/03. SubQuest03_Tumba_Wanted/SubQuest_Wanted04.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CSubQuestNode_Wanted04::Start()
{	
	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pCriminal = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, L"Criminal");

	/* ��ȭ */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->OnOff_DialogWindow(true, CUI_Manager::MINI_DIALOG);
	CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();
}

CBTNode::NODE_STATE CSubQuestNode_Wanted04::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
	{
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
		
		if (!m_bIsCreateSpot && m_pCriminal->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Get_CurrState() == CGameNpc::NPC_STATE::NPC_IDLE)
		{
			m_bIsCreateSpot = true;
			Vec4 vSpotPos = { -49.6f, -5.2f, -40.f , 1.f };
			m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));
		
			CUI_PopupQuest::QUEST_INFO QuestDesc = {};
			QuestDesc.strType = m_strQuestTag;
			QuestDesc.strTitle = m_strQuestName;
			QuestDesc.strContents = m_strQuestContent;
			QuestDesc.bCreateSpot = true;
			QuestDesc.vDestPosition = vSpotPos;
			CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
		}

		if (m_bIsCreateSpot)
		{
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
		}

	}
	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Wanted04::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_Wanted04::TalkEvent()
{
	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Wanted_04_01.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("Stand01Idle01"));
		break;
	case 1:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("Wanted_04_02.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		break;
	}
}

CSubQuestNode_Wanted04* CSubQuestNode_Wanted04::Create()
{
	CSubQuestNode_Wanted04* pInstance = new CSubQuestNode_Wanted04();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Wanted04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Wanted04::Free()
{
	__super::Free();
}
