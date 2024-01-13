#include "stdafx.h"
#include "MainQuestNode_SnowField06.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CMainQuestNode_SnowField06::CMainQuestNode_SnowField06()
{
}

HRESULT CMainQuestNode_SnowField06::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("�屳 �轼���� �����ϱ�");
	m_strQuestContent = TEXT("�轼���� ��������");

	return S_OK;
}

void CMainQuestNode_SnowField06::Start()
{
	CUI_Manager::GetInstance()->Set_QuestPopup(m_strQuestTag, m_strQuestName, m_strQuestContent);

	m_pJackson = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("GrimalKinML01"));
	Vec4 vSpotPos = Set_DestSpot(m_pJackson);

	// �ӽ÷� monster�� 
	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_MONSTER), &vSpotPos));
}

CBTNode::NODE_STATE CMainQuestNode_SnowField06::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_ICELAND)
	{
		if (m_pQuestDestSpot != nullptr)
		{
			m_pQuestDestSpot->Tick(fTimeDelta);
			m_pQuestDestSpot->LateTick(fTimeDelta);

			if (m_pQuestDestSpot != nullptr)
			{
				if (m_pQuestDestSpot->Get_IsCol())
				{
					m_bIsClear = true;
					m_pQuestDestSpot->Set_ReadyDelete(true);
					Safe_Release(m_pQuestDestSpot);
					return NODE_STATE::NODE_FAIL;
				}
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_SnowField06::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_SnowField06* CMainQuestNode_SnowField06::Create()
{
	CMainQuestNode_SnowField06* pInstance = new CMainQuestNode_SnowField06();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_SnowField06");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_SnowField06::Free()
{
	__super::Free();
}
