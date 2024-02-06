#include "stdafx.h"
#include "StelliaState_Rage3Charge.h"

#include "Stellia.h"

#include "GameInstance.h"

#include "Camera_Group.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

CStelliaState_Rage3Charge::CStelliaState_Rage3Charge(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage3Charge::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fChargeTime = .5f;
	m_fLimitTime = 6.f;
	m_fShakeTime = 0.5f;
	m_iClickDest = 30.f;

	return S_OK;
}

void CStelliaState_Rage3Charge::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill06_New_Loop"));
	m_fAccChargeTime = 0.f;
	m_fAccClickTime = 0.f;
	m_fAccShakeTime = 0.5f;
	m_fAccChargeTime = 0.f;

	m_iClickPower = 0;
}

void CStelliaState_Rage3Charge::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	m_fAccChargeTime += fTimeDelta;

	if (m_pStellia->Get_IsPlayerGuardEvent())
	{
		m_pStellia->Get_TargetDesc().pTragetTransform->LookAt_ForLandObject(m_pTransformCom->Get_Position());
		m_pStellia->Get_TargetDesc().pTragetTransform->Move(-m_pStellia->Get_TargetDesc().pTragetTransform->Get_Look(), 3.f, fTimeDelta);

		m_fAccClickTime += fTimeDelta;
		m_fAccShakeTime += fTimeDelta;

		//if (m_fAccShakeTime >= m_fShakeTime)
		//{
			CCamera_Manager::GetInstance()->Start_Action_Shake_Default();
		//	m_fAccShakeTime = m_fShakeTime - m_fAccShakeTime;
		//}

		// ���� ����(�ð� �ʰ�)
		if (m_fAccClickTime >= m_fLimitTime)
		{
			m_pPlayer->Get_CharacterStateCom()->Change_State(CCharacter::DAMAGED_KNOCKDOWN);
			
			/* �÷��̾�� ���ڸ����� ��ġ�� �ݺ��Ͽ� �÷��̾ ���� ���� ���� */
			_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
			Vec4 vDest = m_pStellia->Get_OriginPos();
			vDest -= m_pTransformCom->Get_Position();
			_vector vDestNormal = XMVector3Normalize(vDest);

			_float fDotProduct = XMVectorGetX(XMVector3Dot(vLookNormal, vDestNormal));
			_float fAngle = XMConvertToDegrees(acosf(fDotProduct));

			/* ������ look�� �������� ���Ͱ� ����, �����ʿ� ��ġ�ϴ����� �Ǻ�. */
			_vector vCrossProduct = XMVector3Cross(vLookNormal, vDestNormal);
			_float fCrossProductY = XMVectorGetY(vCrossProduct);

			/* ������ �ٶ󺸴� ������ �������� �����ʿ� ��ġ. */
			if (fCrossProductY > 0.f)
			{
				Vec3 m_vRightRot = XMVector3Rotate(vLookNormal, XMQuaternionRotationRollPitchYaw(0.0f, XMConvertToRadians(45.f), 0.0f));

				m_pStellia->Get_TargetDesc().pTarget->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(m_vRightRot, 10.f, false);
				m_pStellia->Get_TargetDesc().pTarget->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity({ 0.f, 1.f, 0.f }, 10.f, false);
			}
			/* ������ �ٶ󺸴� ������ �������� ���ʿ� ��ġ. */
			else if (fCrossProductY < 0.f)
			{
				Vec3 m_vLeftRot = XMVector3Rotate(vLookNormal, XMQuaternionRotationRollPitchYaw(0.0f, XMConvertToRadians(-45.f), 0.0f));

				m_pStellia->Get_TargetDesc().pTarget->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(m_vLeftRot, 10.f, false);
				m_pStellia->Get_TargetDesc().pTarget->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity({ 0.f, 1.f, 0.f }, 10.f, false);
			}

			m_pStellia->Set_IsPlayerGuardEvent(false);
		}

		// ���� ����
		if (m_iClickPower >= m_iClickDest)
		{
			if (m_iBreakCount < 2)
			{
				m_iBreakCount += 1;
				m_pStellia->Set_IsPlayerGuardEvent(false);
				m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3DAMAGE);
				return;
			}
			else
			{
				// ������ ����
				_float fStunTime = 15.f;
				m_pStateMachineCom->Change_State(CStellia::STELLIA_COUNTERSTART, &fStunTime);
				return;
			}
		}

		// ���� ����(�߰��� ���� ����)
		else if (m_pPlayer->Get_CharacterStateCom()->Get_CurrState() != CCharacter::BATTLE_GUARD)
		{
			dynamic_cast<CCharacter*>(m_pStellia->Get_TargetDesc().pTarget)->Get_CharacterStateCom()->Change_State(CCharacter::DAMAGED_KNOCKDOWN);
			m_pStellia->Get_TargetDesc().pTarget->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(
				-m_pStellia->Get_TargetDesc().pTarget->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_Look(), 10.f, false);

			m_pStellia->Get_TargetDesc().pTarget->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(
				{ 0.f, 1.f, 0.f }, 10.f, false);

			m_pStellia->Set_IsPlayerGuardEvent(false);
		}

		// ��ư Ŭ��
		if (KEY_TAP(KEY::LBTN))
		{
			m_iClickPower += 1;
		}
	}
	else
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fRage3AroundSpeed, fTimeDelta);

	
	// ���ڸ��ư� AroundDist�� �����Ѵٸ� Turn.
	Vec4 vCenterToStellia = m_pStellia->Get_OriginPos() - (Vec4)m_pTransformCom->Get_Position();
	if (fabs(vCenterToStellia.Length()) > m_fAroundDist - 5.f && m_fAccChargeTime >= m_fChargeTime)
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3CHARGE_BREAK);
	}
}

void CStelliaState_Rage3Charge::Exit_State()
{
}

CStelliaState_Rage3Charge* CStelliaState_Rage3Charge::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage3Charge* pInstance = new CStelliaState_Rage3Charge(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage3Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage3Charge::Free()
{
	__super::Free();
}
