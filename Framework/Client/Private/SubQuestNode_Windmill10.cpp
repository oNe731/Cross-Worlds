#include "stdafx.h"
#include "SubQuestNode_Windmill10.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_Fade.h"
#include "UI_PopupQuest.h"
#include "Game_Manager.h"
#include "Building.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

CSubQuestNode_Windmill10::CSubQuestNode_Windmill10()
{
}

HRESULT CSubQuestNode_Windmill10::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("ǳ�� ����");
	m_strQuestContent = TEXT("�����𿡰� ����");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("ǳ�� ����");
	m_strNextQuestContent = TEXT("�����𿡰� ���ư���");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/SubQuest/02. SubQuest02_Verde_WindmillRepair/SubQuest_Windmill10.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CSubQuestNode_Windmill10::Start()
{
	CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MINI_DIALOG);

	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	//m_pKuu = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Kuu"));
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
	m_pVerde = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_NPC, L"Verde");

	m_vecTalker.push_back(m_pKuu);
	m_vecTalker.push_back(m_pVerde);

	/* ��ȭ ī�޶� ���� */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
		pActionCam->Start_Action_Talk(m_pVerde);

	/* ��ȭ */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();

	//m_pChloe = dynamic_cast<CChloe*>(GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Chloe")));
	//Vec4 vSpotPos = Set_DestSpot(m_pChloe);
	//
	//// �ӽ÷� monster�� 
	//m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));
	
	list<CGameObject*>& pGameObjects = GI->Find_GameObjects(LEVELID::LEVEL_EVERMORE, LAYER_TYPE::LAYER_BUILDING);
	auto iter = find_if(pGameObjects.begin(), pGameObjects.end(), [&](CGameObject* pObj) {
		if (true == pObj->IsQuestItem())
			return true;

		return false;
		});

	if (pGameObjects.end() != iter)
	{
		m_pQuestObject = (*iter);

	}
}

CBTNode::NODE_STATE CSubQuestNode_Windmill10::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVELID::LEVEL_EVERMORE)
	{
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
				CUI_PopupQuest::QUEST_INFO QuestDesc = {};
				QuestDesc.strType = m_strNextQuestTag;
				QuestDesc.strTitle = m_strNextQuestName;
				QuestDesc.strContents = m_strNextQuestContent;
				CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
				//			CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MAIN_DIALOG);
				CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, 2.f);

				/* 10, 11, 12 ���� �ϳ��� ������ ���� (���� 12���� ��ȭķ�� ���ش�) */
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

		if (m_bIsFadeOut)
		{
			if (m_bIsFadeOut && CUI_Manager::GetInstance()->Is_FadeFinished())
			{
				m_bIsClear = true;

				return NODE_STATE::NODE_FAIL;
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Windmill10::LateTick(const _float& fTimeDelta)
{
}

void CSubQuestNode_Windmill10::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("00_KuuSay_Hey.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER_FROM_BACK_NPC);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("01_VerdeSay_haha comback.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pVerde->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pVerde->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Verde.ao|Verde_tlk"));
		/* ��ȭ ī�޶� Ÿ�� ���� */
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::NPC); /* Ÿ�� ���� ���� (ǳ�� ī�޶� �׼�)*/
		break;
	}

}

CSubQuestNode_Windmill10* CSubQuestNode_Windmill10::Create()
{
	CSubQuestNode_Windmill10* pInstance = new CSubQuestNode_Windmill10();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Windmill10");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Windmill10::Free()
{
	__super::Free();
}
