#pragma once
#include "GameNpc.h"

BEGIN(Client)

class CHumanFLCapitalMerchant final : public CGameNpc
{
private:
	CHumanFLCapitalMerchant(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CHumanFLCapitalMerchant(const CHumanFLCapitalMerchant& rhs);
	virtual ~CHumanFLCapitalMerchant() = default;

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

private:
	class CUI_World_NPCTag* m_pTag = { nullptr };

public:
	static CHumanFLCapitalMerchant* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

