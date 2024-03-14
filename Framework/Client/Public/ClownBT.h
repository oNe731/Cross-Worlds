#pragma once

#include "Client_Defines.h"
#include "MonsterBT.h"

BEGIN(Client)

class CClown;
class CClownBT final : public CMonsterBT
{
private:
	CClownBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CClownBT(const CClownBT& rhs);
	virtual ~CClownBT() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonster* pOwner);
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

	virtual void     Init_NodeStart();

private:
	_bool	IsZeroHp();
	_bool   IsHit();
	_bool	IsAtk();
	_bool	IsChase();
	_bool	IsReturn();

private:
	CClown* m_pClown = nullptr;

	_bool m_BIsChase = false;


public:
	static CClownBT* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CMonster* pOwner);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
