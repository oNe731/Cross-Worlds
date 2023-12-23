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
		_bool	bDamping = false;

		Vec4	vCurPos;						/* ���� ī�޶� ������ */
		Vec4	vTargetPos;						/* ��ǥ ī�޶� ������ */
		_float	fDampingCoefficient = 0.03f;	/* 0 ~ 1*/

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
	void Set_DampingCoefficient(const _float fCoefficient) { m_tDampingDesc.fDampingCoefficient = fCoefficient; }

protected:
	virtual HRESULT Ready_Components() override;

private:
	Vec4 Calculate_WorldPosition(_float fTimeDelta);
	Vec4 Calculate_LoaclPosition(_float fTimeDelta);
	Vec4 Calculate_Look(_float fTimeDelta);
	Vec4 Calculate_ReleativePosition(Vec4 vPos, Matrix matWorld);

private:
	Vec2			m_vAngle = { 0.f, 1.f };
	
	DAMPING_DESC	m_tDampingDesc = {};

public:
	static CCamera_Follow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END