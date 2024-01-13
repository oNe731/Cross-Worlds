#include "stdafx.h"
#include "SubQuestNode_FindCat03.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CSubQuestNode_FindCat03::CSubQuestNode_FindCat03()
{
}

HRESULT CSubQuestNode_FindCat03::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("����� ã��");
	m_strQuestContent = TEXT("Ŭ������ ����� ã��");

	return S_OK;
}

void CSubQuestNode_FindCat03::Start()
{
	// �̰� ���߿� ���� ����Ʈ �˾� ����� �ű�ٰ� ��ô.
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);
}

CBTNode::NODE_STATE CSubQuestNode_FindCat03::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	// ����̸� ã�ƾ� �ϳ� �ӽ÷� �ϴ� Ű �Է����� �Ѿ��.
	if (KEY_TAP(KEY::N))
	{
		CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);

		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}

	//if (m_pQuestDestSpot != nullptr)
	//{
	//	m_pQuestDestSpot->Tick(fTimeDelta);
	//	m_pQuestDestSpot->LateTick(fTimeDelta);
	//
	//	if (m_pQuestDestSpot != nullptr)
	//	{
	//		if (m_pQuestDestSpot->Get_IsCol())
	//		{
	//			m_bIsClear = true;
	//			m_pQuestDestSpot->Set_Dead(true);
	//			return NODE_STATE::NODE_FAIL;
	//		}
	//	}
	//}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_FindCat03::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_FindCat03* CSubQuestNode_FindCat03::Create()
{
	CSubQuestNode_FindCat03* pInstance = new CSubQuestNode_FindCat03();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_FindCat03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_FindCat03::Free()
{
	__super::Free();
}
