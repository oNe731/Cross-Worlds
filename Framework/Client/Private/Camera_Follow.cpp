#include "stdafx.h"
#include "Camera_Follow.h"

#include "GameInstance.h"

#include "Camera_Manager.h"

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
		m_vLookAtOffset = Vec4::UnitW;
		m_vTargetOffset = Vec4{ 1.2f, 1.3f, 0.f, 1.f };

		m_vMouseSensitivity = Vec2{ 0.18f, 0.5f };
	}

	return S_OK;
}

void CCamera_Follow::Tick(_float fTimeDelta)
{
	if (nullptr == m_pTargetObj || nullptr == m_pLookAtObj)
		return;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, Calculate_WorldPosition(fTimeDelta));
	m_pTransformCom->LookAt(Calculate_Look(fTimeDelta));

	__super::Tick(fTimeDelta); /* Shake, Fov, Dist */

	/*static Vec4 vOrigin;
	if (Is_Shake())
	{
		Vec4 vPos = vOrigin;

		Vec3 vShakeLocalPos = Get_ShakeLocalPos();
		vPos.x += vShakeLocalPos.x;
		vPos.y += vShakeLocalPos.y;
		vPos.z += vShakeLocalPos.z;

		m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vPos);
	}

	if (KEY_TAP(KEY::H))
	{
		vOrigin = m_pTransformCom->Get_Position();;
		Start_Shake(0.01f, 15.f, 0.5f);
	}*/
}

void CCamera_Follow::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pTargetObj || nullptr == m_pLookAtObj)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Follow::Render()
{
	return S_OK;
}

HRESULT CCamera_Follow::Ready_Components()
{
	return S_OK;
}

Vec4 CCamera_Follow::Calculate_WorldPosition(_float fTimeDelta)
{
	/* ���� ���� ��ġ (������ 1 ����) */
	Vec4 vLocalSpherical = Calculate_LoaclSphericalPosition(fTimeDelta);

	/* ���Ͻ� �ݿ� */
	vLocalSpherical *= m_tLerpDist.fCurValue;
	
	/* ī�޶� ��ǥ ���� ��ġ  */
	CTransform* pTargetTransform = m_pTargetObj->Get_Component<CTransform>(L"Com_Transform");

	if (nullptr == pTargetTransform)
		return Vec4::UnitW;

	Vec4 vWorldGoal = vLocalSpherical
		+ Vec4(pTargetTransform->Get_Position());											 /* Ÿ�� ������ */
		+ Calculate_ReleativePosition(m_vTargetOffset, pTargetTransform->Get_WorldMatrix()); /* Ÿ���� ȸ���� �ݿ��� ������ */

	vWorldGoal.w = 1.f;

	/* ���� ���� ���� ��ġ (ī�޶��� ���� ��ġ�� ��ǥ��ġ�� ���� ����� ���� �����Ѵ�)*/
	if (m_tDampingDesc.bDamping)
		return Calculate_DampingPosition(vWorldGoal);

	return vWorldGoal;
}

Vec4 CCamera_Follow::Calculate_LoaclSphericalPosition(_float fTimeDelta)
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

		if (m_vAngle.y <= 0.7f) /* Min : 0.f */
		{
			m_vAngle.y = 0.7f;
		}
		else if (2.5f < m_vAngle.y) /* Max : 3.14*/
		{
			m_vAngle.y = 2.5f;
		}
	}

	/* ���� ��ǥ��(����ǥ��) -> �޼� ���� ��ǥ�� */
	Vec4 vCamLocal
	{
		1.f * sinf(m_vAngle.y) * cosf(m_vAngle.x),	// x = r * sin(���� �Ӱ�) * cos(�浵 ������)
		1.f * cosf(m_vAngle.y),						// y = r * cos(���� �Ӱ�)
		1.f * sinf(m_vAngle.y) * sinf(m_vAngle.x),	// z = r * sin(���� �Ӱ�) * sin(�浵 ������)
		1.f
	};

	return vCamLocal;
}

Vec4 CCamera_Follow::Calculate_Look(_float fTimeDelta)
{
	CTransform* pTargetTransform = m_pTargetObj->Get_Component<CTransform>(L"Com_Transform");

	Vec4 vPosition_LookAtObject = Vec4(pTargetTransform->Get_Position());

	Vec4 vPosition_Offset = Calculate_ReleativePosition(m_vTargetOffset, m_pTransformCom->Get_WorldMatrix()); /* ī�޶��� ȸ�����¸� �ݿ��� Ÿ�� ������ */

	return Vec4(vPosition_LookAtObject + vPosition_Offset).OneW();
}

Vec4 CCamera_Follow::Calculate_ReleativePosition(Vec4 vPos, Matrix matWorld)
{
	/* �������Ŀ��� ȸ�����¸��� �ݿ��Ͽ� �������� �����Ѵ�. */

	/* ����� ������ �ʱ�ȭ */
	matWorld.Translation(Vec3::Zero);

	/* ����� ����� ����ȭ */
	matWorld.Right(XMVector3Normalize(matWorld.Right()));
	matWorld.Up(XMVector3Normalize(matWorld.Up()));
	matWorld.Backward(XMVector3Normalize(matWorld.Backward()));
	
	/* ����� ȸ�� ����� ������ �� */
	Vec3 vRelativePos = XMVector3TransformCoord(vPos, matWorld);

	return Vec4(vRelativePos.x, vRelativePos.y, vRelativePos.z, 1.f);
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
		Vec4 vDist = (vGoalPos.ZeroW() - m_tDampingDesc.vCurPos.ZeroW()) * m_tDampingDesc.fDampingCoefficient;
		
		m_tDampingDesc.vCurPos += vDist;

		return m_tDampingDesc.vCurPos.OneW();
	}
	
	return vGoalPos;
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

	Safe_Release(m_pTransformCom);
}
