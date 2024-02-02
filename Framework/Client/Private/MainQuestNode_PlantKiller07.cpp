#include "stdafx.h"
#include "MainQuestNode_PlantKiller07.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "Quest_Manager.h"

#include "GameNpc.h"

CMainQuestNode_PlantKiller07::CMainQuestNode_PlantKiller07()
{
}

HRESULT CMainQuestNode_PlantKiller07::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("��� ���Ѷ�");
	m_strQuestContent = TEXT("���͵��� ������ ��� ��Ű��");

	// ���� �߰��߰� ��� ��� ����ֱ� = ���̺� ���� Ŭ������ ���� ������ �ű⿡. ������ ���⿡.
	//Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/06.MainQuest_WitchForest/MainQuest_WitchForest05_2.json");
	//
	//for (const auto& talkDesc : Load) {
	//	TALK_DELS sTalkDesc;
	//	sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
	//	sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
	//	m_vecTalkDesc.push_back(sTalkDesc);
	//}

	return S_OK;
}

void CMainQuestNode_PlantKiller07::Start()
{
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);
}

CBTNode::NODE_STATE CMainQuestNode_PlantKiller07::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	// �ӽ�
	// ���Ŀ��� ȣ�� �̺�Ʈ�� ����Ǹ� �Ѿ��. ���� ����Ʈ �˾��� ù ���� ���̺� ��ȯ �� �ٲ��ش�.
	if (KEY_TAP(KEY::X))
	{
		CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);
		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_PlantKiller07::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_PlantKiller07* CMainQuestNode_PlantKiller07::Create()
{
	CMainQuestNode_PlantKiller07* pInstance = new CMainQuestNode_PlantKiller07();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_PlantKiller07");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_PlantKiller07::Free()
{
	__super::Free();
}
