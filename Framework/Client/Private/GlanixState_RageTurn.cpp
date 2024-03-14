#include "stdafx.h"
#include "GlanixState_RageTurn.h"

#include "Glanix.h"

CGlanixState_RageTurn::CGlanixState_RageTurn(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_RageTurn::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_RageTurn::Enter_State(void* pArg)
{
	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	_vector vDestNormal = XMVector3Normalize(m_pGlanix->Get_TargetDesc().pTragetTransform->Get_Position() - m_pTransformCom->Get_Position());

	/* ������ look�� �÷��̾� ��ġ - ���� ��ġ�� ���� ���� ������ ������ ����. */
	_float fDotProduct = XMVectorGetX(XMVector3Dot(vLookNormal, vDestNormal));
	_float fAngle = XMConvertToDegrees(acosf(fDotProduct));

	/* ���� ȸ������� �ʿ��� ������ �ƴ� ������� 1�������� idle ���·� ��ȯ.*/
	if (fAngle < 20.f)
	{
		_float fWaitTime = 0.5f;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGEIDLE, &fWaitTime);
	}
	/* ȸ���� �ʿ��� ������� */
	else
	{
		/* ������ look�� �������� �÷��̾ ����, �����ʿ� ��ġ�ϴ����� �Ǻ�. */
		_vector vCrossProduct = XMVector3Cross(vLookNormal, vDestNormal);
		_float fCrossProductY = XMVectorGetY(vCrossProduct);

		/* ������ �ٶ󺸴� ������ �������� �����ʿ� ��ġ. */
		if (fCrossProductY > 0.f)
		{
			if (fAngle >= 0.f && fAngle < 90.f)
			{
				m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_RightTurn"));
				m_fTurnSpeed = 2.f;
			}
			else if (fAngle >= 90.f && fAngle < 180.f)
			{
				m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_RightTurn180"));
				m_fTurnSpeed = 4.f;
			}
		}
		/* ������ �ٶ󺸴� ������ �������� ���ʿ� ��ġ. */
		else if (fCrossProductY < 0.f)
		{

			if (fAngle >= 0.f && fAngle < 90.f)
			{
				m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_LeftTurn"));
				m_fTurnSpeed = -2.f;
			}
			else if (fAngle >= 90.f && fAngle < 180.f)
			{
				m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_LeftTurn180"));
				m_fTurnSpeed = -4.f;
			}
		}
	}

	m_vDestPos = m_pGlanix->Get_TargetDesc().pTragetTransform->Get_Position();
}

void CGlanixState_RageTurn::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	_vector vDestNormal = XMVector3Normalize(m_vDestPos - m_pTransformCom->Get_Position());

	/* ������ look�� �������� �÷��̾ ����, �����ʿ� ��ġ�ϴ����� �Ǻ�. */
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
		_float fWaitTime = 0.5f;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGEIDLE, &fWaitTime);
	}
}

void CGlanixState_RageTurn::Exit_State()
{
}

CGlanixState_RageTurn* CGlanixState_RageTurn::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_RageTurn* pInstance = new CGlanixState_RageTurn(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_RageTurn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_RageTurn::Free()
{
	__super::Free();
}

