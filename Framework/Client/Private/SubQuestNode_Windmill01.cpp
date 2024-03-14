#include "stdafx.h"
#include "SubQuestNode_Windmill01.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"

CSubQuestNode_Windmill01::CSubQuestNode_Windmill01()
{
}

HRESULT CSubQuestNode_Windmill01::Initialize()
{
	__super::Initialize();

	return S_OK;
}

void CSubQuestNode_Windmill01::Start()
{
	m_pVerde = GI->Find_GameObject(LEVELID::LEVEL_EVERMORE, LAYER_NPC, TEXT("Verde"));
	Vec4 vSpotPos = Set_DestSpot(m_pVerde);

	// �ӽ÷� monster�� 
	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));
}

CBTNode::NODE_STATE CSubQuestNode_Windmill01::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_EVERMORE)
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

void CSubQuestNode_Windmill01::LateTick(const _float& fTimeDelta)
{
}

CSubQuestNode_Windmill01* CSubQuestNode_Windmill01::Create()
{
	CSubQuestNode_Windmill01* pInstance = new CSubQuestNode_Windmill01();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CSubQuestNode_Windmill01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSubQuestNode_Windmill01::Free()
{
	__super::Free();
}
