#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_InGame_Setting_Window final : public CUI
{
protected:
	CUI_InGame_Setting_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_InGame_Setting_Window(const CUI_InGame_Setting_Window& rhs);
	virtual ~CUI_InGame_Setting_Window() = default;

public:
	virtual void Set_Active(_bool bActive) override;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	_bool m_bShowInfo = { false }; // �����찡 ��ǥ ��ġ���� �����ϸ� true�� ���Ѵ� -> �ΰ�, ��ư Active

	_float2 m_vOnPosition = _float2(0.f, 0.f); // UI�� �̵��� ���� ��ġ
	_float2 m_vOffPosition = _float2(0.f, 0.f); // UI�� Active�Ǵ� ������ ������ ��ġ

	_bool m_bTabOpen[3];
	
private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Update_SettingTab();

public:
	static CUI_InGame_Setting_Window* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
