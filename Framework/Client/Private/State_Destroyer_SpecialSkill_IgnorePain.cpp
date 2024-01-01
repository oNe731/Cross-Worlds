#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Destroyer_SpecialSkill_IgnorePain.h"

CState_Destroyer_SpecialSkill_IgnorePain::CState_Destroyer_SpecialSkill_IgnorePain(CStateMachine* pMachine)
    : CState_Character(pMachine)
{
}

HRESULT CState_Destroyer_SpecialSkill_IgnorePain::Initialize(const list<wstring>& AnimationList)
{
    if (FAILED(__super::Initialize(AnimationList)))
        return E_FAIL;
    
    return S_OK;
}

void CState_Destroyer_SpecialSkill_IgnorePain::Enter_State(void* pArg)
{
    m_pCharacter->Appear_Weapon();
    m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Destroyer_SpecialSkill_IgnorePain::Tick_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
        m_pStateMachineCom->Change_State(CCharacter::STATE::BATTLE_IDLE);
}

void CState_Destroyer_SpecialSkill_IgnorePain::Exit_State()
{
    
}

CState_Destroyer_SpecialSkill_IgnorePain* CState_Destroyer_SpecialSkill_IgnorePain::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
    CState_Destroyer_SpecialSkill_IgnorePain* pInstance = new CState_Destroyer_SpecialSkill_IgnorePain(pStateMachine);

    if (FAILED(pInstance->Initialize(AnimationList)))
    {
        MSG_BOX("Fail Create : CState_SwordMan_SpecialSkill_AcaneBarrier");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CState_Destroyer_SpecialSkill_IgnorePain::Free()
{
    __super::Free();
}
