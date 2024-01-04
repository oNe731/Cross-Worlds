#include "..\Public\Camera.h"
#include "GameInstance.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag, _uint iObjectID)
	: CGameObject(pDevice, pContext, strObjTag, iObjectID)
{
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
{

}

HRESULT CCamera::Initialize_Prototype()
{
	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_tProjDesc, pArg, sizeof(PROJ_DESC));

	return S_OK;
}

void CCamera::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	Tick_Lerp(fTimeDelta);
	Tick_Shake(fTimeDelta);
}

void CCamera::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
}

HRESULT CCamera::Render()
{
	if (!m_bActive)
		return S_OK;

	return S_OK;
}

void CCamera::Start_Lerp_Fov(const _float& fStartValue, const _float& fTargetValue, const _float& fTime, const LERP_MODE& eMode)
{
	m_tProjDesc.fPrevFov = m_tProjDesc.tLerpFov.fCurValue;

	m_tProjDesc.tLerpFov.Start(fStartValue, fTargetValue, fTime, eMode);
}

void CCamera::Start_Lerp_Fov(const _float& fTargetValue, const _float& fTime, const LERP_MODE& eMode)
{
	m_tProjDesc.fPrevFov = m_tProjDesc.tLerpFov.fCurValue;

	m_tProjDesc.tLerpFov.Start(m_tProjDesc.tLerpFov.fCurValue, fTargetValue, fTime, eMode);
}

void CCamera::Start_Lerp_Distance(const _float& fStartValue, const _float& fTargetValue, const _float& fTime, const LERP_MODE& eMode)
{
	m_fPrevDist = m_tLerpDist.fCurValue;

	m_tLerpDist.Start(fStartValue, fTargetValue, fTime, eMode);
}

void CCamera::Start_Lerp_Distance(const _float& fTargetValue, const _float& fTime, const LERP_MODE& eMode)
{
	m_fPrevDist = m_tLerpDist.fCurValue;

	m_tLerpDist.Start(m_tLerpDist.fCurValue, fTargetValue, fTime, eMode);
}

void CCamera::Change_TargetOffSet(const Vec4& vTargetValue, const _float& fTime, const LERP_MODE& eMode)
{
	m_tTargetOffset.Start(m_tTargetOffset.vCurVec, vTargetValue, fTime, eMode);
}

void CCamera::Change_LookAtOffSet(const Vec4& vTargetValue, const _float& fTime, const LERP_MODE& eMode)
{
	m_tLookAtOffset.Start(m_tLookAtOffset.vCurVec, vTargetValue, fTime, eMode);
}

void CCamera::Lerp_TargetOffset(const Vec4& vStartValue, const Vec4& vTargetValue, const _float& fTime, const LERP_MODE& eMode)
{
	m_tTargetOffset.Start(vStartValue, vTargetValue, fTime, eMode);
}

void CCamera::Lerp_LookAtOffSet(const Vec4& vStartValue, const Vec4& vTargetValue, const _float& fTime, const LERP_MODE& eMode)
{
	m_tLookAtOffset.Start(vStartValue, vTargetValue, fTime, eMode);
}

void CCamera::Change_TargetObj(CGameObject* pTargetObj, const _float& fChangeTime, const LERP_MODE& eMode)
{
	if (nullptr == m_pTargetObj || nullptr == pTargetObj)
		return;

	Vec4 vSrc	= m_pTargetObj->Get_Component<CTransform>(L"Com_Transform")->Get_Position();
	Vec4 vDest	= pTargetObj->Get_Component<CTransform>(L"Com_Transform")->Get_Position();

	m_tBlendingTargetPosition.Start(vSrc, vDest, fChangeTime, eMode);

	/* ������ ���� �Ǵ� ������ m_tBlendingTargetPosition���� ������ ��� */
	m_pTargetObj = pTargetObj;
}

void CCamera::Change_LookAtObj(CGameObject* pLookAtObj, const _float& fChangeTime, const LERP_MODE& eMode)
{
	if (nullptr == m_pLookAtObj || nullptr == pLookAtObj)
		return;

	Vec4 vSrc	= m_pLookAtObj->Get_Component<CTransform>(L"Com_Transform")->Get_Position();
	Vec4 vDest	= pLookAtObj->Get_Component<CTransform>(L"Com_Transform")->Get_Position();

	m_tBlendingLookAtPosition.Start(vSrc, vDest, fChangeTime, eMode);

	/* ������ ���� �Ǵ� ������ m_tBlendingLookAtPosition���� ������ ��� */
	m_pLookAtObj = pLookAtObj;
}

void CCamera::Start_Shake(const _float& fAmplitude, const _float& fFrequency, const _float& fDuration)
{
	if (0 >= fAmplitude || 0 >= fFrequency || 0 >= fDuration)
		return;

	m_tShakeDesc.bActive = true;

	m_tShakeDesc.fAmplitude = fAmplitude;
	m_tShakeDesc.fFrequency = fFrequency;
	m_tShakeDesc.fDuration = fDuration;

	m_tShakeDesc.fFreqDelta = m_tShakeDesc.fDuration / m_tShakeDesc.fFrequency;

	const _uint iRand = rand() % MAX_UNIT_RAND;

	const Vec3 vTargetPos = { v2UnitRand[iRand].x, v2UnitRand[iRand].y, 0.f };

	m_tShakeDesc.tLerpShakeUnitPos.Start(Vec3::Zero, vTargetPos, m_tShakeDesc.fFreqDelta, LERP_MODE::SMOOTHER_STEP);
}

void CCamera::Tick_Lerp(const _float fDeltaTime)
{
	if (m_tProjDesc.tLerpFov.bActive)
		m_tProjDesc.tLerpFov.Update(fDeltaTime);
	
	if (m_tLerpDist.bActive)
		m_tLerpDist.Update(fDeltaTime);
	
	if (m_tTargetOffset.bActive)
		m_tTargetOffset.Update_Lerp(fDeltaTime);

	if (m_tLookAtOffset.bActive)
		m_tLookAtOffset.Update_Lerp(fDeltaTime);

	if (m_tBlendingTargetPosition.bActive)
		m_tBlendingTargetPosition.Update_Lerp(fDeltaTime);

	if (m_tBlendingLookAtPosition.bActive)
		m_tBlendingLookAtPosition.Update_Lerp(fDeltaTime);
}

void CCamera::Tick_Shake(const _float fDeltaTime)
{
	if (m_tShakeDesc.bActive)
	{
		m_tShakeDesc.fAccDuration += fDeltaTime;
		m_tShakeDesc.fAccFrequency += fDeltaTime;

		/* ���� ���� üũ */
		if (m_tShakeDesc.fDuration <= m_tShakeDesc.fAccDuration)
		{
			/* �ٷ� �� �����°� �ƴ϶� ���� �ʿ� */

			/* ���� �� �ƿ� Ÿ�� ���� ���� */
			m_tShakeDesc.Clear();
			return;
		}

		/* ��ǥ ��ġ ���� */
		if (m_tShakeDesc.fFreqDelta <= m_tShakeDesc.fAccFrequency)
		{
			m_tShakeDesc.fAccFrequency -= m_tShakeDesc.fFreqDelta;

			Vec3 vStartPos, vTargetPos;

			vStartPos = m_tShakeDesc.tLerpShakeUnitPos.vCurVec;

			/* ���� ������ �󵵶�� �������� ���� */
			if (m_tShakeDesc.fDuration <= m_tShakeDesc.fAccFrequency + m_tShakeDesc.fFreqDelta)
				vTargetPos = Vec3::Zero;
			else
			{
				const _uint iRand = rand() % MAX_UNIT_RAND;
				vTargetPos = { v2UnitRand[iRand].x, v2UnitRand[iRand].y, 0.f };
			}

			m_tShakeDesc.tLerpShakeUnitPos.Start(vStartPos, vTargetPos, m_tShakeDesc.fFreqDelta, LERP_MODE::SMOOTHER_STEP);
		}
		else /* ��ġ ���� */
		{
			m_tShakeDesc.tLerpShakeUnitPos.Update_Lerp(fDeltaTime);
		}
	}
}

HRESULT CCamera::Ready_Components()
{
	return S_OK;
}

void CCamera::Free()
{
	__super::Free();
}
