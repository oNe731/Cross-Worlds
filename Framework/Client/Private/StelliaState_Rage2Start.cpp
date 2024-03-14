#include "stdafx.h"
#include "StelliaState_Rage2Start.h"

#include "Stellia.h"

#include "GameInstance.h"

#include "Camera_Group.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

#include "Effect_Manager.h"
#include "Vfx.h"
#include "Vfx_Stellia_Skill_Roar.h"

CStelliaState_Rage2Start::CStelliaState_Rage2Start(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage2Start::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage2Start::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkillRage"));

	// Effect Create
	CVfx* pVfxEffect = nullptr;
	GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Stellia_Skill_Roar"), m_pTransformCom->Get_WorldMatrix(), m_pStellia, &pVfxEffect);
	if (nullptr != pVfxEffect)
		pVfxEffect->Set_OwnerStateIndex((_int)CStellia::STELLIA_RAGE2START);
}

void CStelliaState_Rage2Start::Tick_State(_float fTimeDelta)
{
	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE2LOOP);
	}
}

void CStelliaState_Rage2Start::Exit_State()
{
	/* ũ����Ż ���� */
	m_pStellia->Create_Crystals();
	__super::Set_LockOffStellia();

	/* Camera - �ȷο� -> ž�� */
	//CCamera_Top* pCamTop = dynamic_cast<CCamera_Top*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::TOP));
	//if (nullptr != pCamTop)
	//{
	//	if (S_OK == pCamTop->Start_TopView(CCamera_Top::VIEW_TYPE::GLANIX_PILLAR_PATTERN))
	//	{
	//		CCamera_Manager::GetInstance()->Change_Camera(pCamTop->Get_Key(), 1.25f, LERP_MODE::SMOOTHER_STEP);
	//
	//		// �÷��̾� ��ǲ ����
	//		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CCharacter::STATE::NEUTRAL_IDLE);
	//		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(false);
	//	}
	//}
}

CStelliaState_Rage2Start* CStelliaState_Rage2Start::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage2Start* pInstance = new CStelliaState_Rage2Start(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage2Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage2Start::Free()
{
	__super::Free();
}
