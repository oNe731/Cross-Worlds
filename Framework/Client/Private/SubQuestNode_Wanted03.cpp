#include "stdafx.h"
#include "SubQuestNode_Wanted03.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CSubQuestNode_Wanted03::CSubQuestNode_Wanted03()
{
}

HRESULT CSubQuestNode_Wanted03::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("���� ���");
	m_strQuestContent = TEXT("�ձ��� ���ϳ��� ��ƶ�.");

	return S_OK;
}

void CSubQuestNode_Wanted03::Start()
{
	// ���� ����Ʈ ����� ���� ����Ʈ�� 
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

	/* ���� ����� �߰��Ǹ� ����. */
	//m_pTumba = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("BlackSmithMaster"));
	//Vec4 vSpotPos = Set_DestSpot(m_pTumba);
	//
	//// �ӽ÷� monster�� 
	//m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_MONSTER), &vSpotPos));
}

CBTNode::NODE_STATE CSubQuestNode_Wanted03::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	/* �ӽ� */
	if (KEY_TAP(KEY::N))
	{
		CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);

		m_bIsClear = true;
		return NODE_STATE::NODE_FAIL;
	}

	/* ���� ����� �߰��Ǹ� ����. */
	//if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
	//{
	//	if (m_pQuestDestSpot != nullptr)
	//	{
	//		m_pQuestDestSpot->Tick(fTimeDelta);
	//		m_pQuestDestSpot->LateTick(fTimeDelta);
	//
	//		if (m_pQuestDestSpot != nullptr)
	//		{
	//			if (m_pQuestDestSpot->Get_IsCol())
	//			{
	//				m_bIsClear = true;
	//				m_pQuestDestSpot->Set_Dead(true);
	//				return NODE_STATE::NODE_FAIL;
	//			}
	//		}
	//	}
	//}

	return NODE_STATE::NODE_RUNNING;
}

void CSubQuestNode_Wanted03::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_Wanted03* CSubQuestNode_Wanted03::Create()
{
	CSubQuestNode_Wanted03* pInstance = new CSubQuestNode_Wanted03();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Wanted03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Wanted03::Free()
{
	__super::Free();
}
