#include "stdafx.h"
#include "StelliaState_Spawn.h"

#include "Stellia.h"
#include "Effect_Manager.h"

#include "Animation.h"

#include "Camera_Group.h"

CStelliaState_Spawn::CStelliaState_Spawn(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Spawn::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}
 
void CStelliaState_Spawn::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Spawn"), MIN_TWEEN_DURATION);

	/* ������ ������ ���� ����ϱ� ���� �ִϸ��̼� ��ž */
	m_pModelCom->Set_Stop_Animation(true);

	// ������ ����Ʈ ���� 
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Stellia_Spawn_Roar"), m_pTransformCom->Get_WorldMatrix(), m_pStellia);

	/* Change Bgm */
	GI->Play_BGM(L"BGM_Int_Boss_Halloween_1_Part1.ogg", GI->Get_ChannelVolume(CHANNELID::SOUND_BGM_CURR), false, 0.75f);
}

void CStelliaState_Spawn::Tick_State(_float fTimeDelta)
{
	if (L"SKM_Stellia.ao|Stellia_Spawn" != m_pModelCom->Get_CurrAnimation()->Get_AnimationName())
		return;

	/* ������ ������ ���� ���ð� ī��Ʈ */
	if (!m_bFinishWait)
	{
		m_fAcc += fTimeDelta;
		if (m_fWaitDuration <= m_fAcc)
		{
			m_bFinishWait = true;
			m_pModelCom->Set_Stop_Animation(false);
		}
		else
			return;
	}

	if (m_pPlayer != nullptr)
	{
		if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
		{
			m_pStateMachineCom->Change_State(CStellia::STELLIA_SPAWNIDLE);
		}
	}

	/* ���ڸ��ư� ���� ������ �� ī�޶� ����ŷ �߰� */
	if (30 == m_pModelCom->Get_CurrAnimationFrame())
	{
		CCamera_Manager::GetInstance()->Get_CurCamera()->Start_Shake(0.2f, 17.f, 0.3f);
	}
}

void CStelliaState_Spawn::Exit_State()
{
}

CStelliaState_Spawn* CStelliaState_Spawn::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Spawn* pInstance = new CStelliaState_Spawn(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Spawn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Spawn::Free()
{
	__super::Free();
}

