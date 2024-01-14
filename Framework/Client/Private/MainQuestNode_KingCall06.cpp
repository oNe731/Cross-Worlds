#include "stdafx.h"
#include "MainQuestNode_KingCall06.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Quest_Manager.h"
#include "UI_Manager.h"
#include "Sound_Manager.h"
#include "Camera_Action.h"

#include "Game_Manager.h"

CMainQuestNode_KingCall06::CMainQuestNode_KingCall06()
{
}

HRESULT CMainQuestNode_KingCall06::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("���� �罽��");
	m_strQuestContent = TEXT("�罽���� ��ȭ�ϱ�");

	// ���� ����� Ȯ���ϱ�.
	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/02.MainQuest_KingCall/MainQuest_KingCall06.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_KingCall06::Start()
{
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());

	/* ��ȭ */
	m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
	m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

	CUI_Manager::GetInstance()->OnOff_DialogWindow(true, 1);
	CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

	TalkEvent();

}

CBTNode::NODE_STATE CMainQuestNode_KingCall06::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	m_fTime += fTimeDelta;

	if (m_fTime >= 3.f)
	{
		if (m_iTalkIndex < m_vecTalkDesc.size())
		{
			Safe_Delete_Array(m_szpOwner);
			Safe_Delete_Array(m_szpTalk);

			m_iTalkIndex += 1;

			if (m_iTalkIndex >= m_vecTalkDesc.size())
			{
				CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);

				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, 1);
				return NODE_STATE::NODE_SUCCESS;
			}

			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->Set_MiniDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();

			m_fTime = m_fTalkChangeTime - m_fTime;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_KingCall06::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_KingCall06::TalkEvent()
{
	wstring strAnimName = TEXT("");

	switch (m_iTalkIndex)
	{
		// ��ȭ ���·� ���� �Ű��� �ִϸ��̼� �̸� ������. ���� ����.
		// Talk, Idle, Run, Walk. 
	case 0:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("KuuSay_Intro.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	case 1:
		//CSound_Manager::GetInstance()->Play_Sound(TEXT("KuuSay_Intro.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		break;
	}
}

CMainQuestNode_KingCall06* CMainQuestNode_KingCall06::Create()
{
	CMainQuestNode_KingCall06* pInstance = new CMainQuestNode_KingCall06();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_KingCall06");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_KingCall06::Free()
{
	__super::Free();
}
