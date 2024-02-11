#include "stdafx.h"
#include "GlanixState_Turn.h"

#include "Glanix.h"

CGlanixState_Turn::CGlanixState_Turn(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Turn::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fWalkSpeed = 4.5f;

	return S_OK;
}

void CGlanixState_Turn::Enter_State(void* pArg)
{
	//  ���� �ƴ� �� ���� üũ
	if (m_pGlanix->Get_Stat().fHp <= m_pGlanix->Get_Stat().fMaxHp / 2.f && !m_pGlanix->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK))
	{
		m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK, true);
		m_pGlanix->Set_SkillTree();
		m_iAtkIndex = 0;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_BERSERK);
		return;
	}

	// ������2 ����(������ ��¥ ���ǹ�)
	if (!m_bIsRage2Init && m_pGlanix->Get_Stat().fHp <= m_pGlanix->Get_Stat().fMaxHp * 0.35f &&
		m_pGlanix->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK))
	//if (!m_bIsRage2Init && m_pGlanix->Get_Stat().fHp <= m_pGlanix->Get_Stat().fMaxHp * 0.95f)
	{
		m_bIsRage2Init = true;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGE2START_TURN_WP);
		return;
	}

	// ������1 ����(������ ��¥ ���ǹ�)
	if (!m_bIsRageInit && m_pGlanix->Get_Stat().fHp <= m_pGlanix->Get_Stat().fMaxHp * 0.15f && 
		m_pGlanix->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK))
	//if (!m_bIsRageInit && m_pGlanix->Get_Stat().fHp <= m_pGlanix->Get_Stat().fMaxHp * 0.85f)
	{
		m_bIsRageInit = true;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_RAGESTART_TURN_OC);
		return;
	}

	/* �Ϲ� �� ���� */ 
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_NeutralWalk"));

	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	_vector vDestNormal = XMVector3Normalize(m_pGlanix->Get_TargetDesc().pTragetTransform->Get_Position() - m_pTransformCom->Get_Position());

	/* ������ look�� �÷��̾� ��ġ - ���� ��ġ�� ���� ���� ������ ������ ����. */
	//_float fDotProduct = XMVectorGetX(XMVector3Dot(vLookNormal, vDestNormal));
	//_float fAngle = XMConvertToDegrees(acosf(fDotProduct));

	///* ���� ȸ������� �ʿ��� ������ �ƴ� ������� 3�������� idle ���·� ��ȯ.*/
	//if (fAngle < 20.f)
	//{
	//	_float fWaitTime = 3.f;
	//	m_pStateMachineCom->Change_State(CGlanix::GLANIX_COMBATIDLE, &fWaitTime);
	//}
	/* ȸ���� �ʿ��� ������� */
	//else
	{
		/* ������ look�� �������� Ÿ���� ����, �����ʿ� ��ġ�ϴ����� �Ǻ�. */
		_vector vCrossProduct = XMVector3Cross(vLookNormal, vDestNormal);
		_float fCrossProductY = XMVectorGetY(vCrossProduct);

		/* ������ �ٶ󺸴� ������ �������� �����ʿ� ��ġ. */
		if (fCrossProductY > 0.f)
		{
			m_fTurnSpeed = 3.f;
		}
		/* ������ �ٶ󺸴� ������ �������� ���ʿ� ��ġ. */
		else if (fCrossProductY < 0.f)
		{
			m_fTurnSpeed = -3.f;
		}
	}

	m_vDestPos = m_pGlanix->Get_TargetDesc().pTragetTransform->Get_Position();
}

void CGlanixState_Turn::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	//_vector vDestNormal = XMVector3Normalize(m_vDestPos - m_pTransformCom->Get_Position());
	_vector vDestNormal = XMVector3Normalize(m_pGlanix->Get_TargetDesc().pTragetTransform->Get_Position() - m_pTransformCom->Get_Position());

	/* ������ look�� �������� �÷��̾ ����, �����ʿ� ��ġ�ϴ����� �Ǻ�. */
	_vector vCrossProduct = XMVector3Cross(vLookNormal, vDestNormal);
	_float fCrossProductY = XMVectorGetY(vCrossProduct);

	/* ���������� �� */
	if (m_fTurnSpeed > 0.f)
	{
		if (fCrossProductY > 0.f)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fTurnSpeed, fTimeDelta);
		else
		{
			if (m_iAtkIndex >= m_vecAtkState.size())
				m_iAtkIndex = 0;
			m_pStateMachineCom->Change_State(CGlanix::GLANIX_CHASE);
		}
	}
	/* �������� �� */
	else if (m_fTurnSpeed < 0.f)
	{
		if (fCrossProductY < 0.f)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fTurnSpeed, fTimeDelta);
		else
		{
			if (m_iAtkIndex >= m_vecAtkState.size())
				m_iAtkIndex = 0;
			m_pStateMachineCom->Change_State(CGlanix::GLANIX_CHASE);
		}
	}

	m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fWalkSpeed, fTimeDelta);
	

	//if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	//{
	//	//m_pTransformCom->LookAt_ForLandObject(m_pPlayerTransform->Get_Position());
	//	__super::Start_Pattern();
	//}
}

void CGlanixState_Turn::Exit_State()
{
}

CGlanixState_Turn* CGlanixState_Turn::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Turn* pInstance = new CGlanixState_Turn(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Turn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Turn::Free()
{
	__super::Free();
}

