#pragma once

#include "UI.h"

// ����Ʈ�� ����/�Ϸ� �ϸ� ������ �а� ������ Quest Window

BEGIN(Client)
class CUI_WindowQuest final : public CUI
{
public:
	enum UIQUEST_PROCESS { QUEST_ACCEPT, QUEST_FINISH, QUESTPROCESS_END };

protected:
	CUI_WindowQuest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_WindowQuest(const CUI_WindowQuest& rhs);
	virtual ~CUI_WindowQuest() = default;

public:
	void Set_Active(_bool bActive);
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

private:
	UIQUEST_PROCESS m_eProcessType = { QUESTPROCESS_END };

	_bool m_bActive = { false };
	_bool m_bShowInfo = { false }; // �����찡 ��ǥ ��ġ���� �����ϸ� true�� ���Ѵ� -> �ΰ�, ��ư Active

	_float2 m_vOnPosition = _float2(0.f, 0.f); // UI�� �̵��� ���� ��ġ
	_float2 m_vOffPosition = _float2(0.f, 0.f); // UI�� Active�Ǵ� ������ ������ ��ġ
	
private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_WindowQuest* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
