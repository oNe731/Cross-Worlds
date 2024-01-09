#pragma once
#include "GameNpc.h"

BEGIN(Client)

class CFunyaSnowman final : public CGameNpc
{
private:
	CFunyaSnowman(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CFunyaSnowman(const CFunyaSnowman& rhs);
	virtual ~CFunyaSnowman() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;


public:
	virtual void On_Damaged(const COLLISION_INFO& tInfo) override;

private:
	virtual HRESULT Ready_States() override;
	virtual HRESULT Ready_Colliders() override;

private:
	HRESULT Ready_Sockets();

public:
	static CFunyaSnowman* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

