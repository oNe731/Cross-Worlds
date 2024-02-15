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
	//  ���� �ƴ� �� ���� üũ
	if (m_pStellia->Get_Stat().fHp <= m_pStellia->Get_Stat().fMaxHp / 2.f && !m_pStellia->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK))
	{
		m_pStellia->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK, true);
		m_pStellia->Set_SkillTree();
		m_iAtkIndex = 0;
		m_pStateMachineCom->Change_State(CStellia::STELLIA_BERSERK);
		return;
	}
	
	// ������1 ����
	if (!m_bIsRageInit && m_pStellia->Get_Stat().fHp <= m_pStellia->Get_Stat().fMaxHp * 0.75f)
	{
		m_bIsRageInit = true;
		m_pStellia->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_RAGE, true);
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE1START_TURN_OC);
		return;
	}
	
	// ������2 ���� (������ ��¥ ���ǹ�)
	if (!m_bIsRage2Init && m_pStellia->Get_Stat().fHp <= m_pStellia->Get_Stat().fMaxHp * 0.40f &&
		m_pStellia->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK))
	//if (!m_bIsRage2Init && m_pStellia->Get_Stat().fHp <= m_pStellia->Get_Stat().fMaxHp * 0.90f)
	{
		m_bIsRage2Init = true;
		m_pStellia->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_RAGE2, true);
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE2START_TURN_OC);
		return;
	}
	
	// ������3 ���� (������ ��¥ ���ǹ�)
	if (!m_bIsRage3Init && m_pStellia->Get_Stat().fHp <= m_pStellia->Get_Stat().fMaxHp * 0.15f &&
		m_pStellia->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK))
	//if (!m_bIsRage3Init && m_pStellia->Get_Stat().fHp <= m_pStellia->Get_Stat().fMaxHp * 0.90f)
	{
		m_bIsRage3Init = true;
		m_pStellia->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_RAGE3, true);
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3START_FADEOUT);
		//m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3START_FADEIN);
		return;
	}


	/* �Ϲ� �� ���� */
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Run"));

	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	_vector vDestNormal = XMVector3Normalize(m_pStellia->Get_TargetDesc().pTragetTransform->Get_Position() - m_pTransformCom->Get_Position());

	/* ������ look�� �÷��̾� ��ġ - ���� ��ġ�� ���� ���� ������ ������ ����. */
	_float fDotProduct = XMVectorGetX(XMVector3Dot(vLookNormal, vDestNormal));
	_float fAngle = XMConvertToDegrees(acosf(fDotProduct));

	///* ���� ȸ������� �ʿ��� ������ �ƴ� ������� 3�������� idle ���·� ��ȯ.*/
	//if (fAngle < 20.f)
	//{
	//	_float fWaitTime = 3.f;
	//	m_pStateMachineCom->Change_State(CStellia::GLANIX_COMBATIDLE, &fWaitTime);
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

	m_vDestPos = m_pStellia->Get_TargetDesc().pTragetTransform->Get_Position();
}

void CStelliaState_Turn::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	//_vector vDestNormal = XMVector3Normalize(m_vDestPos - m_pTransformCom->Get_Position());
	_vector vDestNormal = XMVector3Normalize(m_pStellia->Get_TargetDesc().pTragetTransform->Get_Position() - m_pTransformCom->Get_Position());

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
			m_pStateMachineCom->Change_State(CStellia::STELLIA_CHASE);
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
			m_pStateMachineCom->Change_State(CStellia::STELLIA_CHASE);
		}
	}

	m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fRunSpeed, fTimeDelta);
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

