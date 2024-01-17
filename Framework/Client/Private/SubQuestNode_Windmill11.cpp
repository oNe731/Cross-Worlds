#include "stdafx.h"
#include "SubQuestNode_Windmill11.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CSubQuestNode_Windmill11::CSubQuestNode_Windmill11()
{
}

HRESULT CSubQuestNode_Windmill11::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("ǳ�� ����");
	m_strQuestContent = TEXT("�����𿡰� ����");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("ǳ�� ����");
	m_strNextQuestContent = TEXT("�����𿡰� ���ư���");

	return S_OK;
}

void CSubQuestNode_Windmill11::Start()
{
	//m_pChloe = dynamic_cast<CChloe*>(GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Chloe")));
	//Vec4 vSpotPos = Set_DestSpot(m_pChloe);
	//
	//// �ӽ÷� monster�� 
	//m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_MONSTER), &vSpotPos));
}

CBTNode::NODE_STATE CSubQuestNode_Windmill11::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	// ���� �Ϸ��ؾ� �ϳ� �ӽ÷� �ϴ� Ű �Է����� �Ѿ��.
	if (KEY_TAP(KEY::N))
	{
		CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, m_strNextQuestTag, m_strNextQuestName, m_strNextQuestContent);

		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Windmill11::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_Windmill11* CSubQuestNode_Windmill11::Create()
{
	CSubQuestNode_Windmill11* pInstance = new CSubQuestNode_Windmill11();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Windmill11");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Windmill11::Free()
{
	__super::Free();
}
