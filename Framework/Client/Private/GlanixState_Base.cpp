#include "stdafx.h"
#include "GlanixState_Base.h"

#include "GameInstance.h"
#include "Animation.h"

#include "Glanix.h"

_uint CGlanixState_Base::m_iAtkIndex = 0;

CGlanixState_Base::CGlanixState_Base(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{
}

HRESULT CGlanixState_Base::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_pGlanix = dynamic_cast<CGlanix*>(m_pOwner);
	
	/* �Ŀ� �÷��̾�� ��ü */
	for (auto iter : GI->Find_GameObjects(LEVEL_TEST, LAYER_CHARACTER))
	{
		m_pPlayer = dynamic_cast<CCharacter_Dummy*>(iter);
	}
	if(m_pPlayer != nullptr)
		m_pPlayerTransform = m_pPlayer->Get_Component<CTransform>(TEXT("Com_Transform"));

	m_fRunSpeed = 4.f;

	// ������ ��ȯ ��� �� �� �����ϱ�. 

	/* ���� ���ϸ� ���� ��Ƴ���. (�Ŀ� ������ Ȥ�� �������� ��ȯ�ϱ� ����) */
	m_vecAtkState.push_back(CGlanix::GLANIX_ATTACK1);
	m_vecAtkState.push_back(CGlanix::GLANIX_ATTACK2);
	m_vecAtkState.push_back(CGlanix::GLANIX_ICEWAVE);
	m_vecAtkState.push_back(CGlanix::GLANIX_QUADBLOW);
	m_vecAtkState.push_back(CGlanix::GLANIX_SPINBOMB);
	m_vecAtkState.push_back(CGlanix::GLANIX_JUMPSTAMP);

	return S_OK;
}

void CGlanixState_Base::Enter_State(void* pArg)
{
}

void CGlanixState_Base::Tick_State(_float fTimeDelta)
{
	/* Dead */
	if (m_pGlanix->Get_Stat().fHp <= 0.f)
		m_pStateMachineCom->Change_State(CGlanix::GLANIX_DEAD);
}

void CGlanixState_Base::Exit_State()
{
}

void CGlanixState_Base::Init_Pattern()
{
	m_vecAtkState.clear();

	m_vecAtkState.push_back(CGlanix::GLANIX_CHARGE);
	m_vecAtkState.push_back(CGlanix::GLANIX_ICEWAVE);
	m_vecAtkState.push_back(CGlanix::GLANIX_QUADBLOW);
	m_vecAtkState.push_back(CGlanix::GLANIX_SPINBOMBBOMB);
	m_vecAtkState.push_back(CGlanix::GLANIX_SNOWBALL);
	m_vecAtkState.push_back(CGlanix::GLANIX_JUMPSTAMP);
	m_vecAtkState.push_back(CGlanix::GLANIX_ATTACK2);
}

_bool CGlanixState_Base::State_Wait(_float fDestTime, _float fTimeDelta)
{
	if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() != TEXT("SKM_Glanix.ao|Glanix_BattleStand"))
	{
		m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_BattleStand"));
	}

	m_fTime += fTimeDelta;

	if (m_fTime >= fDestTime)
	{
		m_fTime = fDestTime - m_fTime;
		return true;
	}

	return false;
}

void CGlanixState_Base::Free()
{
	__super::Free();
}
