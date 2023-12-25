#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Follow final : public CCamera
{
	typedef struct tagCameraDampingDesc
	{
		/* ������ ȣ���ϸ� ���� ��ġ�� vCurPos�� ��ϵǰ�, �� ������ vTargetPos�� ī�޶� ��ġ�� ����ǰ� */
		/* �� ������ ���� condition��ŭ �������� ������ ���۵ȴ�. */
		_bool	bDamping = true;
		_bool	bSet = false;

		Vec4	vCurPos;						/* ���� ī�޶� ������ */
		Vec4	vTargetPos;						/* ��ǥ ī�޶� ������ */
		_float	fDampingCoefficient			= 0.03f;		/* 0 ~ 1 (���� Ŭ���� ���� ����)*/
		_float	fDampingCoefficientBackMag	= 2.75f;			/* Ÿ���� ��� ī�޶��� ���� �������� ��� DampingCoeff�� ������ (�� ���� �Ѿư�) */
		_float	fDampingBackLimitRad		= 1.57f;		/* ���������� �Ǵ��� ���� ���� */

	}DAMPING_DESC;

private:
	CCamera_Follow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera_Follow(const CCamera_Follow& rhs);
	virtual ~CCamera_Follow() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	/* Access */
	const _float& Get_DampingCoefficient() const { return m_tDampingDesc.fDampingCoefficient; }
	void Set_DampingCoefficient(const _float& fCoefficient) { m_tDampingDesc.fDampingCoefficient = fCoefficient; }

	const _float& Get_DampingCoefficientBackMag() const { return m_tDampingDesc.fDampingCoefficientBackMag; }
	void Set_DampingCoefficientBackMag(const _float& fMag) { m_tDampingDesc.fDampingCoefficientBackMag = fMag; }

	const _float& Get_DampingBackLimitRad() const { return m_tDampingDesc.fDampingBackLimitRad; }
	void Set_DampingBackLimitRad(const _float& fRad) { m_tDampingDesc.fDampingBackLimitRad = fRad; }

	const _bool& Is_Damping() const { return m_tDampingDesc.bDamping; }
	void Set_Damping(const _bool& bDamping) { m_tDampingDesc.bDamping = bDamping; m_tDampingDesc.bSet = bDamping; }

	const _float& Get_MaxRotationLimitDeltaY() const { return m_fMaxRotLimitDeltaY; }
	const _float& Get_MinRotationLimitDeltaY() const { return m_fMinRotLimitDeltaY; }

	void Set_MaxRotationLimitDeltaY(const _float& fLimit) { m_fMaxRotLimitDeltaY = fLimit; }
	void Set_MinRotationLimitDeltaY(const _float& fLimit) { m_fMinRotLimitDeltaY = fLimit; }

protected:
	virtual HRESULT Ready_Components() override;

private:
	Vec4 Calculate_WorldPosition(_float fTimeDelta);
	Vec4 Calculate_LoaclSphericalPosition(_float fTimeDelta);
	Vec4 Calculate_Look(_float fTimeDelta);
	Vec4 Calculate_ReleativePosition(Vec4 vPos, Matrix matWorld);
	Vec4 Calculate_DampingPosition(Vec4 vGoalPos);

private:
	Vec2			m_vAngle				= { 0.f, 1.f };

	/* ȸ������ �ʹ� ���ų� ������� ī�޶� ȹ ���� ��츦 �����ϱ� ���� Limit��*/
	_float			m_fMaxRotLimitDeltaY	= { 0.05f };
	_float			m_fMinRotLimitDeltaY	= { -0.05f };

	DAMPING_DESC	m_tDampingDesc			= {};

	

public:
	static CCamera_Follow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END