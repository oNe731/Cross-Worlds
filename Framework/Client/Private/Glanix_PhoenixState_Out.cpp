#include "stdafx.h"
#include "Glanix_PhoenixState_Out.h"

#include "Glanix_Phoenix.h"

#include "GameInstance.h"
#include "Camera_Manager.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"
#include "Particle_Manager.h"

CGlanix_PhoenixState_Out::CGlanix_PhoenixState_Out(CStateMachine* pStateMachine)
	: CGlanixPhoenixState_Base(pStateMachine)
{
}

HRESULT CGlanix_PhoenixState_Out::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanix_PhoenixState_Out::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Phoenix.ao|Phoenix_Success"));

	m_bEffectCreate = false;
}

void CGlanix_PhoenixState_Out::Tick_State(_float fTimeDelta)
{
	if (false == m_pModelCom->Is_Tween())
	{
		if (!m_bEffectCreate && m_pModelCom->Get_CurrAnimationFrame() >= 30)
		{
			// Effect_Create
			{
				CTransform* pTransform = m_pOwner->Get_Component<CTransform>(L"Com_Transform");
				if (nullptr == pTransform)
					return;

				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Phoenix_Fire"),
					pTransform->Get_WorldMatrix(), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Phoenix_Circles"),
					pTransform->Get_WorldMatrix(), _float3(0.f, 0.7f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
			}

			m_bEffectCreate = true;
		}
		else if (m_pModelCom->Is_Finish())
		{

			Vec4 vGlanixOriginPos = XMVectorSet(-55.f, 1.6f, 363.f, 1.f);
			Vec4 vCharacterPos = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform")->Get_Position();
			Vec4 vCenterPos = (vGlanixOriginPos + vCharacterPos) / 2.f;
			vCenterPos.y = vCharacterPos.y + 0.5f;

			vCenterPos.x = GI->RandomFloat(vCenterPos.x - 5.f, vCenterPos.x + 5.f);
			vCenterPos.z = GI->RandomFloat(vCenterPos.z - 5.f, vCenterPos.z + 5.f);

			if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Glanix_Phoenix"), &vCenterPos)))
			{
				MSG_BOX("Add Glanix_Phoenix Failed.");
				return;
			}
			m_pPhoenix->Set_Dead(true);
		}
	}
}

void CGlanix_PhoenixState_Out::Exit_State()
{
}

CGlanix_PhoenixState_Out* CGlanix_PhoenixState_Out::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanix_PhoenixState_Out* pInstance = new CGlanix_PhoenixState_Out(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanix_PhoenixState_Out");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanix_PhoenixState_Out::Free()
{
	__super::Free();
}

