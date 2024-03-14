#include "stdafx.h"
#include "NpcDMWState_Charge_Ready.h"

#include "GameInstance.h"

#include "DreamMazeWitch_Npc.h"

#include "Animation.h"

#include "Game_Manager.h"
#include "Player.h"

CNpcDMWState_Charge_Ready::CNpcDMWState_Charge_Ready(CStateMachine* pStateMachine)
	: CNpcDMWState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_Charge_Ready::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];
	m_fDist = 5.f;
	m_fMoveTime = 1.f;
	m_fDownTime = 1.f;

	return S_OK;
}

void CNpcDMWState_Charge_Ready::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);

	m_bIsMove = false;
	m_bIsDown = false;
}

void CNpcDMWState_Charge_Ready::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	CGameObject* pPlayer = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

	if (m_pStellia != nullptr && pPlayer != nullptr)
	{

		Vec4 vPlayerPos = pPlayer->Get_Component_Transform()->Get_Position();
		Vec4 vCurPos = m_pTransformCom->Get_Position();

		m_pTransformCom->LookAt_ForLandObject(vPlayerPos);

		// �� ���� ����
		if (!m_bIsMove)
		{
			Vec3 vPlayerLook = pPlayer->Get_Component_Transform()->Get_Look();
			vPlayerLook.Normalize();

			m_vDestPos = vPlayerPos + (vPlayerLook * -m_fDist);
			m_vDestPos.y += 5.f;
			m_tMoveLerp.Start(m_pTransformCom->Get_Position(), m_vDestPos, m_fMoveTime, LERP_MODE::SMOOTH_STEP);

			m_bIsMove = true;
		}

		// �÷��̾ �ٶ󺸸鼭 �̵�
		if (m_tMoveLerp.bActive)
		{
			vCurPos = m_tMoveLerp.Update_Lerp(fTimeDelta);
			m_pTransformCom->Set_Position(vCurPos);
		}
		// �̵��� ��������
		else
		{
			// �ٿ��� ���� ���� ����.
			if (!m_bIsDown)
			{
				m_tDownLerp.Start(vCurPos.y, 0.5f, m_fDownTime, LERP_MODE::SMOOTH_STEP);
				m_bIsDown = true;
			}
			// ������ �Ǿ��ٸ� �ٿ�.
			else
			{
				m_pTransformCom->LookAt_ForLandObject(vPlayerPos);

				if (m_tDownLerp.bActive)
				{
					vCurPos.y = m_tDownLerp.Update(fTimeDelta);
					m_pTransformCom->Set_Position(vCurPos);
				}
				// ��� �̵�, �ٿ��� �������� ���� ����
				else
				{
					m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_CHARGE);
				}
			}
		}
	}
}

void CNpcDMWState_Charge_Ready::Exit_State()
{
}

CNpcDMWState_Charge_Ready* CNpcDMWState_Charge_Ready::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcDMWState_Charge_Ready* pInstance = new CNpcDMWState_Charge_Ready(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcDMWState_Charge_Ready");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcDMWState_Charge_Ready::Free()
{
	__super::Free();
}

