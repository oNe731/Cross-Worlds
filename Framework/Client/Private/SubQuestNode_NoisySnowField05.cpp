#include "stdafx.h"
#include "SubQuestNode_NoisySnowField05.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_Fade.h"
#include "UI_PopupQuest.h"

#include "Game_Manager.h"

#include "CurlingGame_Group.h"

CSubQuestNode_NoisySnowField05::CSubQuestNode_NoisySnowField05()
{
}

HRESULT CSubQuestNode_NoisySnowField05::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("�ò����� �ڿ����� ����");
	m_strQuestContent = TEXT("��ῡ�� �¸��Ͽ� ������Ű��");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("�ò����� �ڿ����� ����");
	m_strNextQuestContent = TEXT("��ῡ�� �¸��Ͽ� ������Ű��");

	return S_OK;
}

void CSubQuestNode_NoisySnowField05::Start()
{
	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strNextQuestTag;
	QuestDesc.strTitle = m_strNextQuestName;
	QuestDesc.strContents = m_strNextQuestContent;
	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);
//	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

	// ���� ���� 
	CCurlingGame_Manager::GetInstance()->Start_Game();

	// ���̵� ��
	CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 2.f);
}

CBTNode::NODE_STATE CSubQuestNode_NoisySnowField05::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;
	
	// ������ ������� ���̵� �ƿ� ��(�ø� ���� �Ŵ������� ó��)���� ���� ���¶�� ���� ���� �Ѿ��. 
	if (CCurlingGame_Manager::GetInstance()->Is_FinishGame())
	{
		if (CUI_Manager::GetInstance()->Is_FadeFinished())
		{
			m_bIsClear = true;
			return NODE_STATE::NODE_FAIL;
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_NoisySnowField05::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_NoisySnowField05* CSubQuestNode_NoisySnowField05::Create()
{
	CSubQuestNode_NoisySnowField05* pInstance = new CSubQuestNode_NoisySnowField05();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_NoisySnowField05");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_NoisySnowField05::Free()
{
	__super::Free();
}
