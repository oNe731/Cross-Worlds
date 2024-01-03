#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Destroyer_Skill_BrutalStrike final : public CVfx
{
protected:
	CVfx_Destroyer_Skill_BrutalStrike(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Destroyer_Skill_BrutalStrike(const CVfx_Destroyer_Skill_BrutalStrike& rhs);
	virtual ~CVfx_Destroyer_Skill_BrutalStrike() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CVfx_Destroyer_Skill_BrutalStrike* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END