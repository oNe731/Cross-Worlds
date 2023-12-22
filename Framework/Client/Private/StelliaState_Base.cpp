#include "stdafx.h"
#include "StelliaState_Base.h"

#include "GameInstance.h"
#include "Animation.h"

#include "Stellia.h"

_uint CStelliaState_Base::m_iAtkIndex = 0;

CStelliaState_Base::CStelliaState_Base(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{
}

HRESULT CStelliaState_Base::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_pStellia = dynamic_cast<CStellia*>(m_pOwner);

	/* �Ŀ� �÷��̾�� ��ü */
	for (auto iter : GI->Find_GameObjects(LEVEL_TEST, LAYER_CHARACTER))
	{
		m_pPlayer = dynamic_cast<CCharacter_Dummy*>(iter);
	}
	if (m_pPlayer != nullptr)
		m_pPlayerTransform = m_pPlayer->Get_Component<CTransform>(TEXT("Com_Transform"));

	m_fRunSpeed = 4.f;

	/* ���� ���ϸ� ���� ��Ƴ���. (�Ŀ� ������ Ȥ�� �������� ��ȯ�ϱ� ����) */
	m_vecAtkState.push_back(CStellia::STELLIA_ATTACK1);
	m_vecAtkState.push_back(CStellia::STELLIA_ATTACK2);
	m_vecAtkState.push_back(CStellia::STELLIA_SPINTAIL);
	m_vecAtkState.push_back(CStellia::STELLIA_JUMPSTAMP);
	m_vecAtkState.push_back(CStellia::STELLIA_LASER);
	m_vecAtkState.push_back(CStellia::STELLIA_CHARGE);

	return S_OK;
}

void CStelliaState_Base::Enter_State(void* pArg)
{
}

void CStelliaState_Base::Tick_State(_float fTimeDelta)
{
	/* Dead */
	if (m_pStellia->Get_Stat().fHp <= 0.f)
		m_pStateMachineCom->Change_State(CStellia::STELLIA_DEAD);
}

void CStelliaState_Base::Exit_State()
{
}

void CStelliaState_Base::Init_Pattern()
{
	m_vecAtkState.clear();

	m_vecAtkState.push_back(CStellia::STELLIA_ATTACK2);
	m_vecAtkState.push_back(CStellia::STELLIA_SPINTAIL);
	m_vecAtkState.push_back(CStellia::STELLIA_JUMPSTAMP);
	m_vecAtkState.push_back(CStellia::STELLIA_TRIPLELASER);
	m_vecAtkState.push_back(CStellia::STELLIA_CHARGE);
	m_vecAtkState.push_back(CStellia::STELLIA_BIGBANG);

}

_bool CStelliaState_Base::State_Wait(_float fDestTime, _float fTimeDelta)
{
	/* ���Ƽ�� �ٸ��� ���� �ִϸ��̼��� �ƴϱ⿡ ���� �������� ó��. */
	if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() != TEXT("SKM_Stellia.ao|Stellia_Stand02"))
	{
		m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Stand02"));
	}

	m_fTime += fTimeDelta;

	if (m_fTime >= fDestTime)
	{
		m_fTime = fDestTime - m_fTime;
		return true;
	}

	return false;
}

void CStelliaState_Base::Free()
{
	__super::Free();
}
