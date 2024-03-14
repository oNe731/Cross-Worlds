#include "stdafx.h"
#include "MainQuestNode_FinalBattle06.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Camera_Manager.h"
#include "Camera_Group.h"

#include "UI_Manager.h"
#include "UI_Fade.h"

#include "Game_Manager.h"
#include "Player.h"

CMainQuestNode_FinalBattle06::CMainQuestNode_FinalBattle06()
{
}

HRESULT CMainQuestNode_FinalBattle06::Initialize()
{
	__super::Initialize();

	return S_OK;
}

void CMainQuestNode_FinalBattle06::Start()
{
	m_pWitch = GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_NPC, TEXT("DreamMazeWitch"));

	if (m_pWitch != nullptr)
	{
		// ���� �ƽ� �ִϸ��̼� ����
		m_pWitch->Get_Component_Model()->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_BossSkillRage01"));
		
		// ���� �ƽ� ī�޶� ���� ����
		CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
		if (nullptr != pActionCam)
			pActionCam->Start_Action_Witch_Roar();
	}
}

CBTNode::NODE_STATE CMainQuestNode_FinalBattle06::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVEL_WITCHFOREST)
	{
		if (m_pWitch != nullptr)
		{
			if (m_pWitch->Get_Component_Model()->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_BossSkillRage01"))
			{
				// ���� ī�޶� ���� �ñ׳�
				if(!m_bCamSignal && m_pWitch->Get_Component_Model()->Get_CurrAnimationFrame() == 70)
				{
					m_bCamSignal = true;
					CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
					if(nullptr != pActionCam)
						pActionCam->Start_Action_Witch_Roar();

					// ����� �� Ȱ��ȭ
					CGame_Manager::GetInstance()->Lerp_RadialBlur(true, true, 0.f, -0.05f, 0.3f, 16.f);
				}

				if(m_pWitch->Get_Component_Model()->Get_CurrAnimationFrame() == 100)
				{
					if (nullptr != CUI_Manager::GetInstance()->Get_Fade())
					{
						m_bIsFadeOut = true;
						CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, 1.f);
					}
				}
			}
		}
		
		if (m_bIsFadeOut && CUI_Manager::GetInstance()->Is_FadeFinished())
		{
			// ����� �� ��Ȱ��ȭ
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_RendererCom()->Set_RadialBlur(false);
			//CGame_Manager::GetInstance()->Lerp_RadialBlur(false, false, -0.05f, 0.f, 0.15f, 16.f);

			m_bIsClear = true;
			CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(false, 1.f);

			return NODE_STATE::NODE_FAIL;
		}
	}


	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_FinalBattle06::LateTick(const _float& fTimeDelta)
{
}

CMainQuestNode_FinalBattle06* CMainQuestNode_FinalBattle06::Create()
{
	CMainQuestNode_FinalBattle06* pInstance = new CMainQuestNode_FinalBattle06();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_FinalBattle06");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_FinalBattle06::Free()
{
	__super::Free();
}
