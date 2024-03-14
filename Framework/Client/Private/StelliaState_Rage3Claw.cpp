#include "stdafx.h"
#include "StelliaState_Rage3Claw.h"

#include "Stellia.h"
#include "Animation.h"

#include "Effect_Manager.h"

CStelliaState_Rage3Claw::CStelliaState_Rage3Claw(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage3Claw::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage3Claw::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Attack01"));

	// Effect Create
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Stellia_Skill_Rage03Claw"), m_pTransformCom->Get_WorldMatrix(), m_pStellia);
}

void CStelliaState_Rage3Claw::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() == 21)
		m_pTransformCom->LookAt_ForLandObject(m_pStellia->Get_TargetDesc().pTragetTransform->Get_Position());

	if (m_pModelCom->Get_CurrAnimationFrame() >= 22 && m_pModelCom->Get_CurrAnimationFrame() <= 27)
	{
		m_pTransformCom->Turn(Vec3(0.f, 1.f, 0.f), -(m_fRunSpeed * 0.5f), fTimeDelta);
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fSprintSpeed * 4.f, fTimeDelta);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3CLAW_ENDCHARGE);
	}
}

void CStelliaState_Rage3Claw::Exit_State()
{
}

CStelliaState_Rage3Claw* CStelliaState_Rage3Claw::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage3Claw* pInstance = new CStelliaState_Rage3Claw(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage3Claw");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage3Claw::Free()
{
	__super::Free();
}

