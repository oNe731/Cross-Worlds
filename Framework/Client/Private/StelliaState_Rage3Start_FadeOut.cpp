#include "stdafx.h"
#include "StelliaState_Rage3Start_FadeOut.h"

#include "Stellia.h"

#include "GameInstance.h"

#include "Camera_Group.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

#include "UI_Manager.h"
#include "UI_Fade.h"

#include "Effect_Manager.h"
#include "Vfx.h"
#include "Vfx_Stellia_Skill_Roar.h"

CStelliaState_Rage3Start_FadeOut::CStelliaState_Rage3Start_FadeOut(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage3Start_FadeOut::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage3Start_FadeOut::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkillRage"));
	m_bIsFadeOut = false;

	// Effect Create
	CVfx* pVfxEffect = nullptr;
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Stellia_Skill_Roar"), m_pTransformCom->Get_WorldMatrix(), m_pStellia, &pVfxEffect);
	if (nullptr != pVfxEffect)
		pVfxEffect->Set_OwnerStateIndex((_int)CStellia::STELLIA_RAGE3START_FADEOUT);

	//CRenderer::FOG_DESC desc;
	//::ZeroMemory(&desc, sizeof(desc));
	//{
	//	desc.fFogDistanceValue = 0.3f;
	//	desc.fFogHeightValue = 0.14f;
	//	desc.fFogStartDepth = -0.650f;
	//	desc.fFogStartDistance = 1.470f;
	//	//desc.fFogHeightDensity = 0.110f;
	//	desc.fFogDistanceDensity = 0.1f;
	//	desc.fFogHeightDensity = 0.66f;
	//}
	//CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_RendererCom()->Set_FogDesc(desc);
	//CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_RendererCom()->Set_FogColor(Vec4(0.33, -0.12f, -0.19f, 1.0f));
}

void CStelliaState_Rage3Start_FadeOut::Tick_State(_float fTimeDelta)
{
	if (GI->Get_CurrentLevel() != LEVELID::LEVEL_TOOL)
	{
		if (m_pModelCom->Get_CurrAnimationFrame() == 85 && !m_bIsFadeOut)
		{
			if (nullptr != CUI_Manager::GetInstance()->Get_Fade())
				CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, 1.f);
			m_bIsFadeOut = true;
		}

		if (CUI_Manager::GetInstance()->Is_FadeFinished() && m_bIsFadeOut)
		{
			m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3START_FADEIN);
		}
	}
	else
	{
		if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
		{
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pStellia->Get_Rage3StartPos());
			m_pTransformCom->LookAt_ForLandObject(m_pStellia->Get_OriginPos());
			m_pStellia->Get_Component<CPhysX_Controller>(TEXT("Com_Controller"))->Set_EnterLevel_Position(m_pTransformCom->Get_Position());

			m_pPlayerTransform->Set_State(CTransform::STATE_POSITION, m_pStellia->Get_OriginPos());
			m_pPlayer->Get_Component<CPhysX_Controller>(TEXT("Com_Controller"))->Set_EnterLevel_Position(m_pPlayerTransform->Get_Position());

			m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3TURN_AROUND);
		}
	}
}

void CStelliaState_Rage3Start_FadeOut::Exit_State()
{
}

CStelliaState_Rage3Start_FadeOut* CStelliaState_Rage3Start_FadeOut::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage3Start_FadeOut* pInstance = new CStelliaState_Rage3Start_FadeOut(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage3Start_FadeOut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage3Start_FadeOut::Free()
{
	__super::Free();
}
