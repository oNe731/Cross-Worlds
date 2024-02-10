#include "stdafx.h"
#include "StelliaState_CombatIdle.h"

#include "Stellia.h"
#include "UI_Manager.h"

CStelliaState_CombatIdle::CStelliaState_CombatIdle(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_CombatIdle::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_bFirst = false;

	return S_OK;
}

void CStelliaState_CombatIdle::Enter_State(void* pArg)
{
	if (m_pStellia->Get_Stat().fHp <= m_pStellia->Get_Stat().fMaxHp / 2.f && !m_pStellia->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK))
	{
		m_pStellia->Set_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_BERSERK, true);
		m_pStellia->Set_SkillTree();
		m_iAtkIndex = 0;
		m_pStateMachineCom->Change_State(CStellia::STELLIA_BERSERK);
		return;
	}
	
	if (false == m_bFirst)
	{
		m_bFirst = true;
		if(LEVEL_TOOL != GI->Get_CurrentLevel())
			CUI_Manager::GetInstance()->OnOff_MiniMap(false);
	}

	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Stand02"));

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

void CStelliaState_CombatIdle::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_TURN);
	}

	//__super::Start_Pattern();

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
	//
	//if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	//{
	//	if (m_pStellia->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_ATKAROUND))
	//	{
	//		if (m_iAtkIndex >= m_vecAtkState.size())
	//			m_iAtkIndex = 0;
	//
	//		m_pStateMachineCom->Change_State(m_vecAtkState[m_iAtkIndex++]);
	//	}
	//	else
	//		m_pStateMachineCom->Change_State(CStellia::STELLIA_CHASE);
	//}
}

void CStelliaState_CombatIdle::Exit_State()
{
	// m_bFirst�� �������� �ʴ´�.
}

CStelliaState_CombatIdle* CStelliaState_CombatIdle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_CombatIdle* pInstance = new CStelliaState_CombatIdle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_CombatIdle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_CombatIdle::Free()
{
	__super::Free();
}

