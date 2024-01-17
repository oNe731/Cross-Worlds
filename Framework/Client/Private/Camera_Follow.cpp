#include "stdafx.h"
#include "Camera_Follow.h"

#include "GameInstance.h"

#include "Camera_Manager.h"

#include "UI_Manager.h"

/* Test */
#include "Camera_CutScene_Map.h"
#include "Camera_Action.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Utils.h"

CCamera_Follow::CCamera_Follow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag, OBJ_TYPE::OBJ_CAMERA)
{
}

CCamera_Follow::CCamera_Follow(const CCamera_Follow & rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Follow::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Follow::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* Set Camera */
	{
		m_tLerpDist.fCurValue = Cam_Dist_Follow_Default;

		/* �ȷο� ī�޶󿡼� ��� �������� ����ϴ� ���� ����. Ÿ�� �����¸��� ����Ѵ�.*/
		m_tTargetOffset.vCurVec = Cam_TargetOffset_Follow_Default;
		m_tLookAtOffset.vCurVec = Cam_LookAtOffset_Follow_Default;

		m_vMouseSensitivity = Vec2{ 0.18f, 0.5f };
	}

	return S_OK;
}

void CCamera_Follow::Tick(_float fTimeDelta)
{
	if (!m_bActive || nullptr == m_pTargetObj || nullptr == m_pLookAtObj)
		return;

	__super::Tick(fTimeDelta); 

	/* Check Exception*/
	if (LOCK_PROGRESS::OFF != m_eLockProgress)
		Check_Exception();

	/* Check Blending */
	if (m_bBlending)
	{
		Tick_Blending(fTimeDelta);
		return;
	}

	/* Check Lock */
	if (LOCK_PROGRESS::FINISH_BLEIDING == m_eLockProgress)
	{
		if (!m_tBlendingLookAtPosition.bActive)
			m_eLockProgress = LOCK_PROGRESS::OFF;
	}

	/* Transform */
	Tick_Transform(fTimeDelta);

	/* Wide View */
	Check_WideView(fTimeDelta);

	/* Test */
	Test(fTimeDelta);
}

void CCamera_Follow::LateTick(_float fTimeDelta)
{
	if (!m_bActive || nullptr == m_pTargetObj || nullptr == m_pLookAtObj)
		return;

	//if (nullptr != m_pControllerCom && !m_bBlending)
	//	m_pControllerCom->LateTick_Controller(fTimeDelta);

	__super::LateTick(fTimeDelta);
}

void CCamera_Follow::Set_Default_Position()
{
	/* ���� ���¶�� */
	if (LOCK_PROGRESS::OFF != m_eLockProgress)
	{
		/* ������ ���� */
		{
			/* ���õ� �ޱ۰����θ� ������ǥ�� ����ϱ� ���� �Ͻ������� ��ǲ�� ����. */
			m_bCanInput = false;
			{
				/* ���� ���� */
				/* Ʈ������ ƽ�� ���� �����ǰ� ���� �ٽ� �����Ѵ�. (��ŸŸ�� �и� ���ɼ� ����) */
				m_tDampingDesc.bSet = false;
				Tick_Transform(GI->Compute_TimeDelta(TIMER_TYPE::GAME_PLAY));

				m_tDampingDesc.bSet = false;
				Tick_Transform(GI->Compute_TimeDelta(TIMER_TYPE::GAME_PLAY));

				m_tDampingDesc.bSet = false;
				Tick_Transform(GI->Compute_TimeDelta(TIMER_TYPE::GAME_PLAY));

				/* ���� �ȷο� ī�޶��� ��� ������ ����� �ȷο� ī�޶��� ȸ����Ŀ� ����ϱ� �����̴�. */
				/* ƽ Ʈ�������� 2���̻� ������� ī�޶��� ȸ����Ŀ��� ��� ��� �������� ��ġ�� ����� ���õȴ�. */
			}
			m_bCanInput = true;
		}
	}
	else
	{
		/* ���� ��ǥ�� �ޱ� �� ��� */
		{
			CTransform* pTargetTransform = m_pTargetObj->Get_Component<CTransform>(L"Com_Transform");

			/* ���� Ÿ���� Y�� ȸ�� ���¸� CW 0 ~ 360�� ��ȯ�Ѵ�. */
			_float fRot = 0.f;
			{
				Vec3 vLook = pTargetTransform->Get_Look();

				_float fAngle = atan2(vLook.x, vLook.z);

				const _float fDegreesPerRadian = 180.0f / DirectX::XM_PI;
				fRot = fAngle * fDegreesPerRadian;
				if (fRot < 0)
					fRot += 360.f;
			}

			/* ȸ�� ���� ���� �÷��̾� �� �ݴ� ���⿡ ī�޶� ��ġ�ϱ� ���� ���� ��ǥ�� �ޱ۰��� ���Ѵ�.*/
			_float result = 0.f;
			{
				if (fRot >= 0 && fRot <= 270) 
					result = -0.5f * XM_PI - fRot * (1.5f * XM_PI / 270.0f);
	
				else if (fRot > 270 && fRot <= 360) 
					result = 0.0f - (fRot - 270) * (0.5f * XM_PI / 90.0f); 
	
				else 
					result = fRot;
			}

			m_vAngle = { result, m_fDefaultAngleY };
		}


		/* ������ ���� */
		{
			/* ���õ� �ޱ۰����θ� ������ǥ�� ����ϱ� ���� �Ͻ������� ��ǲ�� ����. */
			m_bCanInput = false;
			{
				/* ���� ���� */
				/* Ʈ������ ƽ�� ���� �����ǰ� ���� �ٽ� �����Ѵ�. (��ŸŸ�� �и� ���ɼ� ����) */
				m_tDampingDesc.bSet = false;
				Tick_Transform(GI->Compute_TimeDelta(TIMER_TYPE::GAME_PLAY));

				m_tDampingDesc.bSet = false;
				Tick_Transform(GI->Compute_TimeDelta(TIMER_TYPE::GAME_PLAY));

				m_tDampingDesc.bSet = false;
				Tick_Transform(GI->Compute_TimeDelta(TIMER_TYPE::GAME_PLAY));

				/* ���� �ȷο� ī�޶��� ��� ������ ����� �ȷο� ī�޶��� ȸ����Ŀ� ����ϱ� �����̴�. */
				/* ƽ Ʈ�������� 2���̻� ������� ī�޶��� ȸ����Ŀ��� ��� ��� �������� ��ġ�� ����� ���õȴ�. */
			}
			m_bCanInput = true;
		}
	}
}

Vec4 CCamera_Follow::Get_LookAt()
{
	return m_vPrevLookAt;
}

void CCamera_Follow::Set_Blending(const _bool& bBlending)
{
	__super::Set_Blending(bBlending);

	if (!bBlending && !CUI_Manager::GetInstance()->Is_DefaultSettingOn())
	{
		/* Ui On */
		if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
			CUI_Manager::GetInstance()->OnOff_GamePlaySetting(true);
	}

	if (!bBlending)
	{
		if (CAMERA_TYPE::CUTSCENE_MAP == CCamera_Manager::GetInstance()->Get_PrevCamera()->Get_Key())
		{
			/* �� �ƽſ��� ������ �����ٸ� ��� ��ǲ �����ش�. */
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(true);
		}

		if (CAMERA_TYPE::TOP == CCamera_Manager::GetInstance()->Get_PrevCamera()->Get_Key())
		{
			/* ž�信�� ������ �����ٸ� ��� ��ǲ�� �����ش�. */
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(true);
		}
		
		if (CAMERA_TYPE::ACTION == CCamera_Manager::GetInstance()->Get_PrevCamera()->Get_Key())
		{
			CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_PrevCamera());

			if (CCamera_Action::CAMERA_ACTION_TYPE::DOOR == pActionCam->Get_Camera_ActionType())
			{
				/* ���� ī�޶� ���� �׼��̾��ٸ� ��� ��ǲ�� �����ش�. */
				CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(true);
			}
		}
	}
}

void CCamera_Follow::Reset_WideView_To_DefaultView()
{
	/* Check Variable */
	m_fAccForWideView = 0.f;
	m_bWideView = false;

	/* Distance */
	m_tLerpDist.Clear();
	m_tLerpDist.fStartValue = m_tLerpDist.fCurValue = m_tLerpDist.fTargetValue = Cam_Dist_Follow_Default;
	
	/* Fov */
	m_tProjDesc.tLerpFov.Clear();
	m_tProjDesc.tLerpFov.fStartValue = m_tProjDesc.tLerpFov.fCurValue = m_tProjDesc.tLerpFov.fTargetValue = Cam_Fov_Follow_Default;
}

HRESULT CCamera_Follow::Start_LockOn(CGameObject* pTargetObject, const Vec4& vTargetOffset, const Vec4& vLookAtOffset, const _float& fLockOnBlendingTime)
{
	if (nullptr == pTargetObject || LOCK_PROGRESS::OFF != m_eLockProgress)
		return E_FAIL;

	Change_LookAtObj(pTargetObject, fLockOnBlendingTime);

	Change_LookAtOffSet(vLookAtOffset, fLockOnBlendingTime);

	Change_TargetOffSet(vTargetOffset, fLockOnBlendingTime);
	
	m_eLockProgress = LOCK_PROGRESS::START_BLENDING;

	return S_OK;
}

HRESULT CCamera_Follow::Finish_LockOn(CGameObject* pTargetObject, const _float& fLockOnBlendingTime)
{
	if (nullptr == pTargetObject || LOCK_PROGRESS::OFF == m_eLockProgress)
		return E_FAIL;

	Change_LookAtObj(pTargetObject, fLockOnBlendingTime);

	Change_LookAtOffSet(Cam_LookAtOffset_Follow_Default, fLockOnBlendingTime);

	Change_TargetOffSet(Cam_TargetOffset_Follow_Default, fLockOnBlendingTime);

	m_eLockProgress = LOCK_PROGRESS::FINISH_BLEIDING;

	m_bLockLookHeight = false;

	m_fLockLookHeight = 0.f;

	return S_OK;
}

HRESULT CCamera_Follow::Lock_LookHeight()
{
	/* ���Ƽó�� Ÿ���� ���� ������ �� �÷��̾ ȭ�� ������ ������ �ʰ� �� ���̸� ������Ų��. */
	/* ���� Ÿ���� ���̰� �� �������� �������� �ٽ� �ڵ����� �����Ѵ�. */

	if (nullptr == m_pLookAtObj)
		return E_FAIL;

	m_bLockLookHeight = true;

	const Matrix matLookWorld = m_pLookAtObj->Get_Component<CModel>(L"Com_Model")->Get_SocketLocalMatrix(0)
						* m_pLookAtObj->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix();

	m_fLockLookHeight = matLookWorld.m[3][1];

	return S_OK;
}

HRESULT CCamera_Follow::Ready_Components()
{
	/* CPhysX_Controller */
	{
		CPhysX_Controller::CONTROLLER_DESC ControllerDesc;
		{
			ControllerDesc.eType = CPhysX_Controller::CAPSULE;
			ControllerDesc.pTransform = m_pTransformCom;
			ControllerDesc.vOffset = { 0.f, 0.f, 0.f };
			ControllerDesc.fHeight = 0.01f;
			ControllerDesc.fMaxJumpHeight = 10.f;
			ControllerDesc.fRaidus = 0.1f;
			ControllerDesc.pOwner = this;
		}

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_PhysXController"), TEXT("Com_Controller"), (CComponent**)&m_pControllerCom, &ControllerDesc)))
			return E_FAIL;
	}

	if(nullptr != m_pControllerCom)
		m_pControllerCom->Set_Active(true);

	return S_OK;
}

void CCamera_Follow::Tick_Transform(const _float fDeltaTime)
{
	/* Position */
	const Vec4 vCamPos = Calculate_WorldPosition(fDeltaTime);
	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vCamPos);

	/* Look & Shake */
	const Vec4 vLookAtPos = Calculate_Look();
	{
		if (Is_Shake())
			m_vPrevLookAt = Vec4(vLookAtPos + Vec4(Get_ShakeLocalPos())).OneW();
		else
			m_vPrevLookAt = vLookAtPos;

		m_pTransformCom->LookAt(m_vPrevLookAt);
	}

	/* PhysX */
	//if (nullptr != m_pControllerCom && !m_bBlending)
	//	m_pControllerCom->Tick_Controller(fDeltaTime);
}

void CCamera_Follow::Tick_Blending(const _float fDeltaTime)
{
	const Vec4 vCamPosition = CCamera_Manager::GetInstance()->Get_BlendingPosition();

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vCamPosition);

	m_vPrevLookAt = CCamera_Manager::GetInstance()->Get_BlendingLookAt();

	m_pTransformCom->LookAt(m_vPrevLookAt);

	/* PhysX */
	/* ���� ���� �������� ������ */
	//if (nullptr != m_pControllerCom && !m_bBlending)
	//	m_pControllerCom->Tick_Controller(fDeltaTime);

	/* ���� ���۽� ������ �����ϹǷ� ������ ����� ������ ���� ���� ���¸� �����ؾ� �Ѵ�. */
	m_tDampingDesc.bSet = false;
}

Vec4 CCamera_Follow::Calculate_WorldPosition(_float fTimeDelta)
{
	Vec4 vWorldGoal		= Vec4::UnitW;
	Vec4 vTargetOffset	= Vec4::UnitW;

	CTransform* pTargetTransform = m_pTargetObj->Get_Component<CTransform>(L"Com_Transform");

	if (LOCK_PROGRESS::OFF != m_eLockProgress)
	{
		/* ī�޶��� ȸ�� ���¸� �ݿ��� ������ */
		vTargetOffset = m_pTransformCom->Get_RelativeOffset(m_tTargetOffset.vCurVec);
		
		/* ī�޶��� ���� ��� ���� ��ȯ���� ���� �������� y�� -�� �Ǿ� ���� �մ� ���� ����*/
		if (vTargetOffset.y < m_fLockTargetOffsetMinY)
			vTargetOffset.y = m_fLockTargetOffsetMinY;

		/* ī�޶� ��ǥ ���� ��ġ */
		vWorldGoal = vTargetOffset + (Vec4)pTargetTransform->Get_Position();
	}
	else
	{
		/* ���� ���� ��ġ (����, ������ 1 ����) */
		Vec4 vLocalSpherical = Calculate_LoaclSphericalPosition(fTimeDelta);
	
		/* ���Ͻ� �ݿ� */
		vLocalSpherical *= m_tLerpDist.fCurValue;
		
		/* Ÿ���� ȸ�� ���¸� �ݿ��� ������ */
		vTargetOffset = pTargetTransform->Get_RelativeOffset(m_tTargetOffset.vCurVec);

		/* ī�޶� ��ǥ ���� ��ġ */
		vWorldGoal = vLocalSpherical + vTargetOffset + (Vec4)pTargetTransform->Get_Position();											
	}
	vWorldGoal.w = 1.f;

	/* ���� ���� ���� ���� ��ġ (ī�޶��� ���� ��ġ�� ��ǥ��ġ�� ���� ����� ���� �����Ѵ�) */
	if (m_tDampingDesc.bDamping)
		return Calculate_DampingPosition(vWorldGoal);

	return vWorldGoal;
}

Vec4 CCamera_Follow::Calculate_LoaclSphericalPosition(_float fTimeDelta)
{
	if (m_bCanInput) 
	{
		_long	MouseMove = 0l;

		if (MouseMove = GI->Get_DIMMoveState(DIMM_X))
		{
			_float fDelta = MouseMove * m_vMouseSensitivity.y * fTimeDelta * -1.f;
		
			/* y�� ȸ������ �ʹ� ���� ��� ī�޶� ȹ ���� ���� ���� �ϱ� ���� ���� */
			{
				if (fDelta < m_fMinRotLimitDeltaY)
					fDelta = m_fMinRotLimitDeltaY;

				if (m_fMaxRotLimitDeltaY < fDelta)
					fDelta = m_fMaxRotLimitDeltaY;
			}

			m_vAngle.x += fDelta;
		}

		if (MouseMove = GI->Get_DIMMoveState(DIMM_Y))
		{
			m_vAngle.y += MouseMove * m_vMouseSensitivity.x * fTimeDelta;

			if (m_vAngle.y <= m_fMinLimitY) /* Min : 0.f */
			{
				m_vAngle.y = m_fMinLimitY;
			}
			else if (m_fMaxLimitY < m_vAngle.y) /* Max : 3.14*/
			{
				m_vAngle.y = m_fMaxLimitY;
			}
		}
	}

	/* ���� ��ǥ��(����ǥ��) -> �޼� ���� ��ǥ�� */
	const _float fRadius = 1.f;

	Vec4 vCamLocal
	{
		fRadius * sinf(m_vAngle.y) * cosf(m_vAngle.x),	// x = r * sin(���� �Ӱ�) * cos(�浵 ������)
		fRadius * cosf(m_vAngle.y),						// y = r * cos(���� �Ӱ�)
		fRadius * sinf(m_vAngle.y) * sinf(m_vAngle.x),	// z = r * sin(���� �Ӱ�) * sin(�浵 ������)
		1.f
	};

	return vCamLocal;
}

Vec4 CCamera_Follow::Calculate_Look()
{
	Vec4 vLookAt, vLookAtOffset;

	CTransform* pTargetTransform = m_pLookAtObj->Get_Component<CTransform>(L"Com_Transform");

	/* ��� ��ġ */
	if (LOCK_PROGRESS::OFF != m_eLockProgress) /* Lock On, Lock Blending */
	{
		/* ��� ������Ʈ�� ���� ���� ���̶�� */
		if (Is_Blending_LookAtObj()) 
			vLookAt = m_tBlendingLookAtPosition.vCurVec;
		else
		{
			Matrix matLookWorld = m_pLookAtObj->Get_Component<CModel>(L"Com_Model")->Get_SocketLocalMatrix(0)
								* m_pLookAtObj->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix();

			memcpy(&vLookAt, &matLookWorld.m[3], sizeof(Vec4));
			
			/* ���� ���� �ɸ� ��� (���Ƽ) */
			if (m_bLockLookHeight)
			{
				if (vLookAt.y <= m_fLockLookHeight)
					m_bLockLookHeight = false;
				else
					vLookAt.y = m_fLockLookHeight;
			}
		}
	}
	else /* Lock Off */
		vLookAt = Vec4(pTargetTransform->Get_Position());

	/* ��� ������ ��ġ */
	vLookAtOffset = m_pTransformCom->Get_RelativeOffset(m_tLookAtOffset.vCurVec); /* ī�޶��� ȸ�� ���¸� �ݿ��� Ÿ�� ������ */
	
	return Vec4(vLookAt + vLookAtOffset).OneW();
}

Vec4 CCamera_Follow::Calculate_DampingPosition(Vec4 vGoalPos)
{
	if (!m_tDampingDesc.bSet) /* ������ ��������, ���� ������ �� �� ��� �����Ѵ�. */
	{
		m_tDampingDesc.bSet = true;
		m_tDampingDesc.vCurPos = vGoalPos;

		return m_tDampingDesc.vCurPos;
	}
	else /* ������ ������ �̷�� ���ٸ� ���� ����� �����Ѵ�. */
	{
		/* ���� Ÿ���� ��� ī�޶��� ���� �����⿡ ����� ��� ������ �ӵ��� �ø���. */
		Vec4 vTargetLook	= m_pTargetObj->Get_Component<CTransform>(L"Com_Transform")->Get_Look();
		Vec4 vCamLook		= m_pTransformCom->Get_Look();

		vTargetLook.y = vCamLook.y = 0.f;
		
		const _float fRad = acosf(vCamLook.Normalized().Dot(vTargetLook.Normalized()));

		_float fCoeff = 0.f;
		if (m_tDampingDesc.fDampingBackLimitRad <= fRad)
			fCoeff = m_tDampingDesc.fDampingCoefficient * m_tDampingDesc.fDampingCoefficientBackMag;
		else
			fCoeff = m_tDampingDesc.fDampingCoefficient;

		const Vec4 vDist = (vGoalPos.ZeroW() - m_tDampingDesc.vCurPos.ZeroW()) * fCoeff;

		m_tDampingDesc.vCurPos += vDist;

		return m_tDampingDesc.vCurPos.OneW();
	}
	
	return vGoalPos;
}

void CCamera_Follow::Check_Exception()
{
	if (nullptr != m_pTargetObj && m_pTargetObj->Is_ReserveDead())
		m_pTargetObj = nullptr;

	if (nullptr != m_pLookAtObj && m_pLookAtObj->Is_ReserveDead())
		m_pLookAtObj = nullptr;
}

void CCamera_Follow::Check_WideView(_float fTimeDelta)
{
	/* Check Exception */
	{
		/* ����(�������, ����)������ üũ�Ѵ�. (����, �ƽ�, ���� ��� ��ٷӴ�.) */
		if (LEVELID::LEVEL_EVERMORE != GI->Get_CurrentLevel() && LEVELID::LEVEL_KINGDOMHALL != GI->Get_CurrentLevel())
			return;

		/* ���� ���� Ȥ�� ���� ���¿����� üũ���� �ʴ´�. -> ������ ����Ʈ fov, dist�� ���� �Ǿ� �־�� �Ѵ�. */
		if (!m_bBlending && LOCK_PROGRESS::FINISH_BLEIDING == m_eLockProgress)
			return;

		/* fov, distance�� ���� ���̶�� üũ���� �ʴ´�.*/
		if (Is_Lerp_Fov() || Is_Lerp_Distance())
			return;
	}

	CStateMachine* pStateMachine = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CStateMachine>(L"Com_StateMachine");
	if (nullptr == pStateMachine)
		return;

	CCharacter::STATE eCurState = (CCharacter::STATE)pStateMachine->Get_CurrState();

	if (m_bWideView) 
	{
		/* Check For Transition Default View */
		if (CCharacter::STATE::NEUTRAL_RUN != eCurState && CCharacter::STATE::BATTLE_WALK != eCurState && CCharacter::STATE::BATTLE_RUN != eCurState)
		{
			m_fAccForWideView += fTimeDelta;

			if (m_fDefaultViewCheckTime <= m_fAccForWideView)
			{
				Start_Lerp_Fov(Cam_Fov_Follow_Default, m_fWideViewLerpTime, m_eWideViewLerpMode);
				Start_Lerp_Distance(Cam_Dist_Follow_Default, m_fWideViewLerpTime, m_eWideViewLerpMode);

				m_bWideView = false;
				m_fAccForWideView = 0.f;
			}
		}
		else
			m_fAccForWideView = 0.f;
	}
	else
	{
		/* Check For Transition Wide View */
		if (CCharacter::STATE::NEUTRAL_RUN == eCurState || CCharacter::STATE::BATTLE_WALK == eCurState || CCharacter::STATE::BATTLE_RUN == eCurState) 
		{
			m_fAccForWideView += fTimeDelta;

			if (m_fWideViewCheckTime <= m_fAccForWideView)
			{
				m_bWideView = true;
				m_fAccForWideView = 0.f;

				Start_Lerp_Fov(Cam_Fov_Follow_Wide, m_fWideViewLerpTime * 1.25f, m_eWideViewLerpMode);
				Start_Lerp_Distance(Cam_Dist_Follow_Wide, m_fWideViewLerpTime * 1.25f, m_eWideViewLerpMode);

				Lock_Fov(true);
				Lock_Dist(true);
			}
		}
		else
		{
			m_fAccForWideView = 0.f;

			if (Is_Lock_Fov())
				Lock_Fov(false);

			if (Is_Lock_Dist())
				Lock_Dist(false);
		}
	}
}

void CCamera_Follow::Test(_float fTimeDelta)
{
	/* CutScene - Evermore */
	//if (KEY_TAP(KEY::INSERT))
	//{
	//	vector<string> CutSceneNames;
	//	CutSceneNames.push_back("Evermore_Street_00");
	//	CutSceneNames.push_back("Evermore_Street_01");
	//
	//	dynamic_cast<CCamera_CutScene_Map*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE_MAP))->Start_CutScenes(CutSceneNames);
	//}
	
	//if (KEY_TAP(KEY::HOME))
	//{
	//	const _float fLerpTime = 1.f;
	//	const LERP_MODE eLerpMode = LERP_MODE::SMOOTHER_STEP;
	//
	//	Start_Lerp_Distance(40.f, fLerpTime, eLerpMode);
	//	Start_Lerp_Fov(XMConvertToRadians(85.f), fLerpTime, eLerpMode);
	//	Lerp_TargetOffset(Cam_TargetOffset_Follow_Default, Vec4{ 0.7f, 20.f, 0.f, 1.f }, fLerpTime, eLerpMode);
	//	Lerp_LookAtOffSet(Cam_LookAtOffset_Follow_Default, Vec4{ 0.7f, 30.f, 0.f, 1.f }, fLerpTime, eLerpMode);
	//}

	/*if (KEY_TAP(KEY::END_KEY))
	{
		const _float fLerpTime = 1.f;
		const LERP_MODE eLerpMode = LERP_MODE::SMOOTHER_STEP;

		Start_Lerp_Distance(Cam_Dist_Follow_Default, fLerpTime, eLerpMode);
		Start_Lerp_Fov(Cam_Fov_Follow_Default, fLerpTime, eLerpMode);
		Lerp_TargetOffset(Vec4{ 0.7f, 15.f, 0.f, 1.f }, Cam_TargetOffset_Follow_Default, fLerpTime, eLerpMode);
		Lerp_LookAtOffSet(Vec4{ 0.7f, 15.f, 0.f, 1.f }, Cam_LookAtOffset_Follow_Default, fLerpTime, eLerpMode);
	}*/
}

CCamera_Follow * CCamera_Follow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_Follow*		pInstance = new CCamera_Follow(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Follow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Follow::Clone(void* pArg)
{
	CCamera_Follow*		pInstance = new CCamera_Follow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_Follow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Follow::Free()
{
	__super::Free();
	Safe_Release(m_pControllerCom);
	Safe_Release(m_pTransformCom);
}
