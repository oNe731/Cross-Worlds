#include "stdafx.h"
#include "SubQuestNode_Windmill10.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CSubQuestNode_Windmill10::CSubQuestNode_Windmill10()
{
}

HRESULT CSubQuestNode_Windmill10::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("ǳ�� ����");
	m_strQuestContent = TEXT("�����𿡰� ����");

	return S_OK;
}

void CSubQuestNode_Windmill10::Start()
{
	// CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

	//m_pChloe = dynamic_cast<CChloe*>(GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Chloe")));
	//Vec4 vSpotPos = Set_DestSpot(m_pChloe);
	//
	//// �ӽ÷� monster�� 
	//m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_MONSTER), &vSpotPos));
}

CBTNode::NODE_STATE CSubQuestNode_Windmill10::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	// ���� �Ϸ��ؾ� �ϳ� �ӽ÷� �ϴ� Ű �Է����� �Ѿ��.
	if (KEY_TAP(KEY::N))
	{
		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Windmill10::LateTick(const _float& fTimeDelta)
{
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
