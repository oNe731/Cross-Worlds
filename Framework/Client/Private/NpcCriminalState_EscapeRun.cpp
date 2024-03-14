#include "stdafx.h"
#include "NpcCriminalState_EscapeRun.h"

#include "GameNpc.h"

#include "Animation.h"

CNpcCriminalState_EscapeRun::CNpcCriminalState_EscapeRun(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CNpcCriminalState_EscapeRun::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];
	m_fDestBoostTime = 1.5f;

	return S_OK;
}

void CNpcCriminalState_EscapeRun::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CNpcCriminalState_EscapeRun::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	Vec4 vPlayerToDist = m_pPlayerTransform->Get_Position() - m_pTransformCom->Get_Position();

	// ������ ������ Boost
	if (!m_bIsBoost && vPlayerToDist.Length() < 2.f)
	{
		m_fAccBoostTime = 0.f;
		m_bIsBoost = true;
	}
	else
	{
		m_fAccBoostTime += fTimeDelta;
		if (m_fAccBoostTime >= m_fDestBoostTime)
		{
			m_fAccBoostTime = 0.f;
			m_bIsBoost = false;
		}
	}


	// ����
	_float4 vPos;
	_float4 vDestPos;

	XMStoreFloat4(&vPos, m_pTransformCom->Get_Position());
	XMStoreFloat4(&vDestPos, m_pNpc->Get_RoamingIndex(m_pNpc->Get_CurRoamingIndex()));

	// �ڿ������� �� �ϱ� ����(-1�� �� ������ ������ �ε����� ���� �Ͽ��� �� ���� ���� �˻����� �ʱ� ����)
	if (m_pNpc->Get_CurRoamingIndex() < m_pNpc->Get_RoamingArea()->size())
	{
		Vec3 vCurDir = XMVector3Normalize(m_pTransformCom->Get_Look());
		Vec3 vDestDir = XMVector3Normalize(vDestPos - (Vec4)m_pTransformCom->Get_Position());

		_vector vCrossProduct = XMVector3Cross(vCurDir, vDestDir);
		_float fCrossProductY = XMVectorGetY(vCrossProduct);
		
		_float fTurnSpeed = 2.5f;
		
		/* ù��° ���̶�� ������ ���ϵ��� ���ǵ带 ���δ�. */
		if(0 == m_pNpc->Get_CurRoamingIndex())
			fTurnSpeed = 5.f;

		/* ���������� �� */
		if (fCrossProductY > 0.f)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTurnSpeed, fTimeDelta);
		/* �������� �� */
		if (fCrossProductY < 0.f)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTurnSpeed, fTimeDelta);
	}
	//

	// m_pTransformCom->LookAt_ForLandObject(m_pNpc->Get_RoamingIndex(m_pNpc->Get_CurRoamingIndex()));
	
	if (m_bIsBoost)
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_pNpc->Get_Stat()->fSpeed * 2.f , fTimeDelta);
	else
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_pNpc->Get_Stat()->fSpeed, fTimeDelta);

	if (vPos.x >= vDestPos.x - 0.1f && vPos.x <= vDestPos.x + 0.1f &&
		vPos.z >= vDestPos.z - 0.1f && vPos.z <= vDestPos.z + 0.1f)
	{
		m_pNpc->Set_CurRoamingIndex(m_pNpc->Get_CurRoamingIndex() + 1);

		if (m_pNpc->Get_CurRoamingIndex() == m_pNpc->Get_RoamingArea()->size())
		{
			m_pTransformCom->LookAt_ForLandObject(m_pPlayerTransform->Get_Position());
			m_pStateMachineCom->Change_State(CGameNpc::NPC_IDLE);
		}
		
	}
}

void CNpcCriminalState_EscapeRun::Exit_State()
{
}

CNpcCriminalState_EscapeRun* CNpcCriminalState_EscapeRun::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcCriminalState_EscapeRun* pInstance = new CNpcCriminalState_EscapeRun(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcCriminalState_EscapeRun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcCriminalState_EscapeRun::Free()
{
	__super::Free();
}

