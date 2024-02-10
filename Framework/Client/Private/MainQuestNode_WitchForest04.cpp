#include "stdafx.h"
#include "MainQuestNode_WitchForest04.h"

#include "GameInstance.h"
#include "Utils.h"

#include "UI_Manager.h"
#include "UI_PopupQuest.h"
#include "Game_Manager.h"

CMainQuestNode_WitchForest04::CMainQuestNode_WitchForest04()
{
}

HRESULT CMainQuestNode_WitchForest04::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("������ ������ ã�Ƽ�");
	m_strQuestContent = TEXT("������ ������ ã�� �������� ������");

	return S_OK;
}

void CMainQuestNode_WitchForest04::Start()
{
	Vec4 vSpotPos = { 89.f, 1.5f, -9.7f, 1.f };
	m_pQuestDestSpot = dynamic_cast<CQuest_DestSpot*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_Quest_DestSpot"), _uint(LAYER_ETC), &vSpotPos));
	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strQuestTag;
	QuestDesc.strTitle = m_strQuestName;
	QuestDesc.strContents = m_strQuestContent;
	QuestDesc.bCreateSpot = true;
	QuestDesc.vDestPosition = vSpotPos;
	CUI_Manager::GetInstance()->Set_QuestPopup(&QuestDesc);

	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());
}

CBTNode::NODE_STATE CMainQuestNode_WitchForest04::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_WITCHFOREST)
	{
		if (m_pQuestDestSpot != nullptr)
		{
			m_pQuestDestSpot->Tick(fTimeDelta);
			m_pQuestDestSpot->LateTick(fTimeDelta);

			if (m_pQuestDestSpot->Get_IsCol())
			{
				CUI_Manager::GetInstance()->Clear_QuestPopup(m_strQuestName);

				m_bIsClear = true;
				m_pQuestDestSpot->Set_ReadyDelete(true);
				Safe_Release(m_pQuestDestSpot);

				return NODE_STATE::NODE_FAIL;
			}
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_WitchForest04::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_WitchForest04::TalkEvent()
{
}

CMainQuestNode_WitchForest04* CMainQuestNode_WitchForest04::Create()
{
	CMainQuestNode_WitchForest04* pInstance = new CMainQuestNode_WitchForest04();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_WitchForest04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_WitchForest04::Free()
{
	Safe_Release(m_pTalkSpot);
	__super::Free();
}
