#include "stdafx.h"
#include "StelliaState_Turn.h"

#include "Stellia.h"

CStelliaState_Turn::CStelliaState_Turn(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Turn::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Turn::Enter_State(void* pArg)
{
	if (m_pStellia->Get_Stat().fHp <= m_pStellia->Get_Stat().fMaxHp / 2.f && !m_pStellia->Get_IsRage())
	{
		m_pStellia->Set_IsRage(true);
		m_pStellia->Set_SkillTree();
		m_iAtkIndex = 0;
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE);
		return;
	}

	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	_vector vDestNormal = XMVector3Normalize(m_pPlayerTransform->Get_Position() - m_pTransformCom->Get_Position());

	/* ������ look�� �÷��̾� ��ġ - ���� ��ġ�� ���� ���� ������ ������ ����. */
	_float fDotProduct = XMVectorGetX(XMVector3Dot(vLookNormal, vDestNormal));
	_float fAngle = XMConvertToDegrees(acosf(fDotProduct));

	/* ���� ȸ������� �ʿ��� ������ �ƴ� ������� 1�������� idle ���·� ��ȯ.*/
	if (fAngle < 20.f)
	{
		_float fWaitTime = 1.f;
		m_pStateMachineCom->Change_State(CStellia::STELLIA_COMBATIDLE, &fWaitTime);
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

	m_vDestPos = m_pPlayerTransform->Get_Position();
}

void CStelliaState_Turn::Tick_State(_float fTimeDelta)
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
		if (m_pStellia->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND))
		{
			if (m_iAtkIndex >= m_vecAtkState.size())
				m_iAtkIndex = 0;

			m_pStateMachineCom->Change_State(m_vecAtkState[m_iAtkIndex++]);
		}
		else
			m_pStateMachineCom->Change_State(CStellia::STELLIA_CHASE);

	}
}

void CStelliaState_Turn::Exit_State()
{
}

CStelliaState_Turn* CStelliaState_Turn::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Turn* pInstance = new CStelliaState_Turn(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Turn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Turn::Free()
{
	__super::Free();
}

