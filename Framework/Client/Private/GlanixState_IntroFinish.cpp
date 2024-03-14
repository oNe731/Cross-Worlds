#include "stdafx.h"
#include "GlanixState_IntroFinish.h"

#include "Glanix.h"

#include "Animation.h"

#include "Camera_Manager.h"
#include "Camera_CutScene_Boss.h"
#include "UI_Manager.h"

#include "UI_Manager.h"
#include "UI_Fade.h"

#include "GameInstance.h"
#include "Effect_Manager.h"

#include "Game_Manager.h"
#include "Player.h"

CGlanixState_IntroFinish::CGlanixState_IntroFinish(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_IntroFinish::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_bFadeOut = false;

	return S_OK;
}

void CGlanixState_IntroFinish::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_IntroFinish01"), 0.3f);

	// Effect Create
	{
		CTransform* pTransformCom = m_pGlanix->Get_Component<CTransform>(L"Com_Transform");
		if (pTransformCom == nullptr)
			return;
		GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Glanix_Intro_Finish"), pTransformCom->Get_WorldMatrix(), m_pGlanix);
	}
}

void CGlanixState_IntroFinish::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() == 5)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pGlanix->Get_OriginPos());
		m_pGlanix->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Set_Velocity({ 0.f, 0.f, 0.f });

		/* Camera */
		{
			CCamera_CutScene_Boss* pCutSceneCam = dynamic_cast<CCamera_CutScene_Boss*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE_BOSS));
			if (nullptr != pCutSceneCam)
				pCutSceneCam->Send_Signal();
		}
	}

	if (!m_pModelCom->Is_Tween() && 0.95f <= m_pModelCom->Get_Progress() && !m_bFadeOut)
	{
		/* Start Fade Out */
		if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
			CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, m_fFadeOutTime, false);

		m_bFadeOut = true;
	}

	if (m_bFadeOut)
	{
		m_fAcc += fTimeDelta;
		if (m_fWaitTime <= m_fAcc)
		{
			m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
		}
	}
}

void CGlanixState_IntroFinish::Exit_State()
{
	/* Camera */
	CCamera_CutScene_Boss* pCutSceneCam = dynamic_cast<CCamera_CutScene_Boss*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE_BOSS));
	if (nullptr != pCutSceneCam)
	{
		/* Start Fade In */
		if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
			CUI_Manager::GetInstance()->Get_Fade()->Renew_Fade(false, m_fFadeInTime, false);

		/* On UI */
		if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
			CUI_Manager::GetInstance()->OnOff_GamePlaySetting(true);
		// �̴ϸ��� ����
		if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
			CUI_Manager::GetInstance()->OnOff_MiniMap(false);

		/* Finish CutScene */
		if (FAILED(pCutSceneCam->Finish_CutScene()))
			return;

		/* Player Input On */
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(true);
	}
}

CGlanixState_IntroFinish* CGlanixState_IntroFinish::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_IntroFinish* pInstance = new CGlanixState_IntroFinish(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_IntroFinish");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_IntroFinish::Free()
{
	__super::Free();
}

