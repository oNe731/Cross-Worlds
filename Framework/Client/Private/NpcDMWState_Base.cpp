#include "stdafx.h"
#include "NpcDMWState_Base.h"

#include "GameInstance.h"
#include "Animation.h"

#include "DreamMazeWitch_Npc.h"

#include "Game_Manager.h"
#include "Player.h"

_uint CNpcDMWState_Base::m_iAtkIndex = 0;

CNpcDMWState_Base::CNpcDMWState_Base(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{
}

HRESULT CNpcDMWState_Base::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_pWitch = dynamic_cast<CDreamMazeWitch_Npc*>(m_pOwner);
	m_pPlayer = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

	if (m_pPlayer != nullptr)
		m_pPlayerTransform = m_pPlayer->Get_Component<CTransform>(TEXT("Com_Transform"));

	m_fFollowingSpeed = 12.f;

	/* ���� ���ϸ� ���� ��Ƴ���. (�Ŀ� ������ Ȥ�� �������� ��ȯ�ϱ� ����) */
	m_vecAtkState.push_back(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_ATTACK);
	// m_vecAtkState.push_back(CDreamMazeWitch_Npc::WITCHSTATE_BATTLE_LASER);

	m_fCreateParticleTime = 0.1f;

	return S_OK;
}

void CNpcDMWState_Base::Enter_State(void* pArg)
{

}

void CNpcDMWState_Base::Tick_State(_float fTimeDelta)
{
	if (m_pStellia == nullptr)
	{
		m_pStellia = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_MONSTER, TEXT("Stellia"));
	}
}

void CNpcDMWState_Base::Exit_State()
{
}

void CNpcDMWState_Base::Following_Stellia(_float fTimeDelta)
{
	if (m_pStellia != nullptr)
	{
		m_pTransformCom->LookAt_ForLandObject(m_pPlayer->Get_Component_Transform()->Get_Position());
		Vec4 vStelliaPos = m_pStellia->Get_Component_Transform()->Get_Position();
		Vec4 vReleativePos = {};

		if (m_pStellia->Get_Component_StateMachine()->Get_CurrState() == CStellia::STELLIA_RAGE1LOOP_EXPLOSION)
		{
			vReleativePos = m_pStellia->Get_Component_Transform()->Get_RelativeOffset({ 0.f , 8.f, 0.f, 1.f });
		}
		else
		{
			vReleativePos = m_pStellia->Get_Component_Transform()->Get_RelativeOffset({ -5.f , 8.f, -5.f, 1.f });
		}

		Vec4 vDestPos = Vec4(vReleativePos + vStelliaPos).OneW();

		if (Vec4::Zero == m_vCurPos || m_fDampingLimitDistance < Vec4::Distance(vDestPos, m_vCurPos))
		{
			m_vCurPos = vDestPos;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(m_vCurPos, 1.f));
		}
		else
		{
			const Vec4 vDist = (vDestPos.ZeroW() - m_vCurPos.ZeroW()) * m_fDampingCoefficient;
			m_vCurPos += vDist;
			m_vCurPos.y = vDestPos.y;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(m_vCurPos, 1.f));
		}

	}
}

void CNpcDMWState_Base::Free()
{
	__super::Free();
}
