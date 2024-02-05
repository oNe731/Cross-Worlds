#include "stdafx.h"
#include "StelliaState_SpinTail.h"

#include "Animation.h"
#include "Stellia.h"

#include "Effect_Manager.h"
#include "Decal.h"

CStelliaState_SpinTail::CStelliaState_SpinTail(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_SpinTail::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_SpinTail::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill01"));

	// Effect Create
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Stellia_Skill_SpinTail"), m_pTransformCom->Get_WorldMatrix(), m_pStellia);
}

void CStelliaState_SpinTail::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() == TEXT("SKM_Stellia.ao|Stellia_BossSkill01"))
	{
		if (m_pModelCom->Get_CurrAnimationFrame() <= 50)
			m_pStellia->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, true);

		else
			m_pStellia->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_COUNTER, false);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_COMBATIDLE);
		// m_pStateMachineCom->Change_State(CStellia::STELLIA_SPINTAIL);
	}
}

void CStelliaState_SpinTail::Exit_State()
{
}

CStelliaState_SpinTail* CStelliaState_SpinTail::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_SpinTail* pInstance = new CStelliaState_SpinTail(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_SpinTail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_SpinTail::Free()
{
	__super::Free();
}

