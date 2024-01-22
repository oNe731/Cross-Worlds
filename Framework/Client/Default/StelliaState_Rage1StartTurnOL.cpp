#include "stdafx.h"
#include "StelliaState_Rage1StartTurnOL.h"

#include "Stellia.h"

CStelliaState_Rage1StartTurnOL::CStelliaState_Rage1StartTurnOL(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage1StartTurnOL::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage1StartTurnOL::Enter_State(void* pArg)
{
	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	_vector vDestNormal = XMVector3Normalize(m_pStellia->Get_OriginLook());

	_float fDotProduct = XMVectorGetX(XMVector3Dot(vLookNormal, vDestNormal));
	_float fAngle = XMConvertToDegrees(acosf(fDotProduct));

	/* ������ look�� �������� ���Ͱ� ����, �����ʿ� ��ġ�ϴ����� �Ǻ�. */
	_vector vCrossProduct = XMVector3Cross(vLookNormal, vDestNormal);
	_float fCrossProductY = XMVectorGetY(vCrossProduct);

	/* ������ �ٶ󺸴� ������ �������� �����ʿ� ��ġ. */
	if (fCrossProductY > 0.f)
	{
		if (fAngle >= 0.f && fAngle < 90.f)
		{
			m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_RightTurn"));
			m_fTurnSpeed = 2.f;
		}
		else if (fAngle >= 90.f && fAngle < 180.f)
		{
			m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_RightTurn180"));
			m_fTurnSpeed = 4.f;
		}
	}
	/* ������ �ٶ󺸴� ������ �������� ���ʿ� ��ġ. */
	else if (fCrossProductY < 0.f)
	{

		if (fAngle >= 0.f && fAngle < 90.f)
		{
			m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_LeftTurn"));
			m_fTurnSpeed = -2.f;
		}
		else if (fAngle >= 90.f && fAngle < 180.f)
		{
			m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_InstantTurn"));
			m_fTurnSpeed = -4.f;
		}
	}
}

void CStelliaState_Rage1StartTurnOL::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	_vector vDestNormal = XMVector3Normalize(m_pStellia->Get_OriginLook());

	/* ������ look�� �������� ���Ͱ� ����, �����ʿ� ��ġ�ϴ����� �Ǻ�. */
	_vector vCrossProduct = XMVector3Cross(vLookNormal, vDestNormal);
	_float fCrossProductY = XMVectorGetY(vCrossProduct);

	/* ���������� �� */
	if (m_fTurnSpeed > 0.f)
	{
		if (fCrossProductY > 0.f)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fTurnSpeed, fTimeDelta);
	}
	/* �������� �� */
	else if (m_fTurnSpeed < 0.f)
	{
		if (fCrossProductY < 0.f)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fTurnSpeed, fTimeDelta);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE1START);
	}
}

void CStelliaState_Rage1StartTurnOL::Exit_State()
{
}

CStelliaState_Rage1StartTurnOL* CStelliaState_Rage1StartTurnOL::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage1StartTurnOL* pInstance = new CStelliaState_Rage1StartTurnOL(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage1StartTurnOL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage1StartTurnOL::Free()
{
	__super::Free();
}

