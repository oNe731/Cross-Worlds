#include "stdafx.h"
#include "GlanixState_CombatIdle.h"

#include "Glanix.h"

CGlanixState_CombatIdle::CGlanixState_CombatIdle(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_CombatIdle::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_CombatIdle::Enter_State(void* pArg)
{
	if (m_pGlanix->Get_Stat().fHp <= m_pGlanix->Get_Stat().fMaxHp / 2.f && !m_pGlanix->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK))
	{
		m_pGlanix->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK, true);
		m_pGlanix->Set_SkillTree();
		m_iAtkIndex = 0;
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_BERSERK);
		return;
	}

	if (pArg != nullptr)
		m_fWaitTime = *(_float*)pArg;

	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BattleStand"));
	m_fTime = 0.f;

	//_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	//_vector vDestNormal = XMVector3Normalize(m_pPlayerTransform->Get_Position() - m_pTransformCom->Get_Position());
	//
	///* ������ look�� �÷��̾� ��ġ - ���� ��ġ�� ���� ���� ������ ������ ����. */
	//_float fDotProduct = XMVectorGetX(XMVector3Dot(vLookNormal, vDestNormal));
	//_float fAngle = XMConvertToDegrees(acosf(fDotProduct));
	//
	///* ������ look�� �������� �÷��̾ ����, �����ʿ� ��ġ�ϴ����� �Ǻ�. */
	//_vector vCrossProduct = XMVector3Cross(vLookNormal, vDestNormal);
	//_float fCrossProductY = XMVectorGetY(vCrossProduct);
	//
	///* ������ �ٶ󺸴� ������ �������� �����ʿ� ��ġ. */
	//if (fCrossProductY > 0.f)
	//{
	//	m_fTurnSpeed = 2.f;
	//}
	///* ������ �ٶ󺸴� ������ �������� ���ʿ� ��ġ. */
	//else if (fCrossProductY < 0.f)
	//{
	//	m_fTurnSpeed = -2.f;
	//}
	//
	//m_vDestPos = m_pPlayerTransform->Get_Position();
}

void CGlanixState_CombatIdle::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	// m_pTransformCom->LookAt_ForLandObject(m_pPlayerTransform->Get_Position());

	m_fTime += fTimeDelta;

	//_vector vLookNormal = XMVector3Normalize(m_pTransformCom->Get_Look());
	//_vector vDestNormal = XMVector3Normalize(m_vDestPos - m_pTransformCom->Get_Position());
	//
	///* ������ look�� �������� �÷��̾ ����, �����ʿ� ��ġ�ϴ����� �Ǻ�. */
	//_vector vCrossProduct = XMVector3Cross(vLookNormal, vDestNormal);
	//_float fCrossProductY = XMVectorGetY(vCrossProduct);
	//
	///* ���������� �� */
	//if (m_fTurnSpeed > 0.f)
	//{
	//	if (fCrossProductY > 0.f)
	//		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fTurnSpeed, fTimeDelta);
	//}
	///* �������� �� */
	//else if (m_fTurnSpeed < 0.f)
	//{
	//	if (fCrossProductY < 0.f)
	//		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fTurnSpeed, fTimeDelta);
	//}

	if (m_fTime >= m_fWaitTime)
	{
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_TURN);
		//__super::Start_Pattern();
	}
}

void CGlanixState_CombatIdle::Exit_State()
{
}

CGlanixState_CombatIdle* CGlanixState_CombatIdle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_CombatIdle* pInstance = new CGlanixState_CombatIdle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_CombatIdle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_CombatIdle::Free()
{
	__super::Free();
}

