#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_SubMenu_Shop final : public CUI
{
public:
	enum UI_SUBSHOP_TYPE
	{ SHOPBTN_PACKAGE, SHOPBTN_GACHA, SHOPBTN_TOKEN, SHOPMENU_END };
	// ����, ��ȯ, ��ǥ ��ȯ

protected:
	CUI_SubMenu_Shop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag, UI_SUBSHOP_TYPE eUIType);
	CUI_SubMenu_Shop(const CUI_SubMenu_Shop& rhs);
	virtual ~CUI_SubMenu_Shop() = default;

public: // Get/Set �Լ�
	virtual void Set_Active(_bool bActive) override;
	_bool Get_Active() { return m_bActive; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	virtual void On_MouseEnter(_float fTimeDelta) override;
	virtual void On_Mouse(_float fTimeDelta) override;
	virtual void On_MouseExit(_float fTimeDelta) override;

public:
	void Reset_InitializeInfo();

private: 
	UI_SUBSHOP_TYPE m_eUIType = { UI_SUBSHOP_TYPE::SHOPMENU_END };

	_bool m_bClicked = { false }; // ���õǾ���
	_bool m_bArrived = { false }; // ��ǥ ��ġ���� �����ߴ�
	_bool m_bUsable = { false }; // ����� �� �ִ� �������ΰ�? -> ����� �ִ°�
	_bool m_bUpdateAlpha = { false };

	_float2 m_vOnPosition = _float2(0.f, 0.f); // Active�� ���� UI�� ������ġ
	_float2 m_vGoalPosition = _float2(0.f, 0.f);
	_float m_fSpeed = { 0.f };

	_int m_iTextureIndex = { -1 };
	_float m_fTimeAcc = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT Ready_Buttons();

	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_SubMenu_Shop* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext,
		const wstring& strObjectTag, UI_SUBSHOP_TYPE eUIType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
