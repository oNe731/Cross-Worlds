#include "stdafx.h"
#include "GlanixState_RageStamp.h"

#include "Glanix.h"
#include "GameInstance.h"

#include "Animation.h"

#include "Camera_Manager.h"
#include "Camera_Follow.h"

#include "Game_Manager.h"
#include "Character.h"
#include "Player.h"
#include "StateMachine.h"
#include "Glanix_IcePillar_Controller.h"

#include "Effect_Manager.h"

CGlanixState_RageStamp::CGlanixState_RageStamp(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageStamp::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_RageStamp::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_RageFinalStamp"));
	m_bDownEffectCreate = false;
}

void CGlanixState_RageStamp::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Get_CurrAnimationFrame() == 15)
	{
		/* Camera - ž�� -> �ȷο� */
		const CAMERA_TYPE eCamType = CAMERA_TYPE::FOLLOW;
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(eCamType));
		if (nullptr != pFollowCam && eCamType != CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Key())
		{
			pFollowCam->Set_Default_Position();

			/* �ʹ� ������ �ϸ� ��� ��ġ ���� �޶��� */
			CCamera_Manager::GetInstance()->Change_Camera(pFollowCam->Get_Key(), 0.5f, LERP_MODE::SMOOTHER_STEP);

			/* ���� ��ǲ�� �ݾ��ش�..*/
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CCharacter::STATE::NEUTRAL_IDLE);
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_Move_Input(false);
		}


		/* ��� ���� ���� */
		{
			const Vec4 vOrigin = m_pGlanix->Get_OriginPos();

			const Vec4 vPlayerPos = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CTransform>(L"Com_Transform")->Get_Position();

			const _float fDist = Vec4(vPlayerPos - vOrigin).xyz().Length();

			if (nullptr != m_pGlanix->Get_PillarsController() && fDist <= m_pGlanix->Get_PillarsController()->Get_DeathDistnace())
				CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Decrease_HP(999);
		}

		if (false == m_bDownEffectCreate)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Glanix_Skill_RageStamp"), m_pTransformCom->Get_WorldMatrix(), m_pGlanix);

			_float4x4 matCrack = {};
			matCrack._11 = m_pGlanix->Get_PillarsController()->Get_DecalSize();
			matCrack._22 = 1.f;
			matCrack._33 = m_pGlanix->Get_PillarsController()->Get_DecalSize();

			matCrack._41 = m_pGlanix->Get_OriginPos().x;
			matCrack._42 = 2.f;
			matCrack._43 = m_pGlanix->Get_OriginPos().z;
			matCrack._44 = 1.f;

			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_Rage01_Stamp"),
				XMLoadFloat4x4(&matCrack), Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), Vec3(0.f, 0.f, 0.f));
		
			m_bDownEffectCreate = true;
		}

		// Warning ��Į ����
		if(m_pGlanix->Get_PillarsController() != nullptr)
			m_pGlanix->Get_PillarsController()->PillarControllerFree();
	}

	else if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_STUN);
	}
}

void CGlanixState_RageStamp::Exit_State()
{
}

CGlanixState_RageStamp* CGlanixState_RageStamp::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageStamp* pInstance = new CGlanixState_RageStamp(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageStamp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageStamp::Free()
{
	__super::Free();
}

