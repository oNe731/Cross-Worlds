#include "stdafx.h"
#include "SubQuestNode_NoisySnowField05.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_Fade.h"

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
	CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

	// ���� ���� �Լ� �߰�
	CCurlingGame_Manager::GetInstance()->Start_Game();

	// ���̵� ��
	CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 2.f);
}

CBTNode::NODE_STATE CSubQuestNode_NoisySnowField05::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;
	
	if (CCurlingGame_Manager::GetInstance()->Finish_Game()) // ���� ������ �����ٸ� 
	{
		// ���̵�� �ø� ���� �Ŵ��� �ʿ��� ó���Ѵ�. 
		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;

		//// ���̵� �ƿ� ����
		//if (!m_bFadeOut)
		//{
		//	CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, 3.f);
		//	m_bFadeOut = true;
		//}
	}

	// ���̵� �ƿ��� �����ٸ� ���� ���� �Ѿ��.
	//if (m_bFadeOut)
	//{
	//	if (CUI_Manager::GetInstance()->Is_FadeFinished())
	//	{
	//		m_bIsClear = true;
	//		return NODE_STATE::NODE_FAIL;
	//	}
	//}

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
