#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Quest_Reward_Item final : public CUI
{
public:
	enum UI_QUESTREWARD_ITEM { REWARD_EXP, REWARD_COIN, REWARD_HPPOTION, /*REWARD_ENERGY,*/ REWARD_UDADAK, QUESTITEM_END };

public:
	typedef struct tagRewardsInformation
	{
		// �������� �ִ� 4������ ������ �� �ִ�.
		_bool bFirstSlot = true; // ù��° ������ ����� ���ΰ�
		UI_QUESTREWARD_ITEM eFirstItem = UI_QUESTREWARD_ITEM::REWARD_COIN; // ù��° ���� ������
		_int iFirstAmount = 10000; // ������ ����(��)

		_bool bSecondSlot = false; // �ι�° ������ ����� ���ΰ�
		UI_QUESTREWARD_ITEM eSecondItem = UI_QUESTREWARD_ITEM::QUESTITEM_END;
		_int iSecondAmount = 0;

		_bool bThirdSlot = false;
		UI_QUESTREWARD_ITEM eThirdItem = UI_QUESTREWARD_ITEM::QUESTITEM_END;
		_int iThirdAmount = 0;

		_bool bFourthSlot = false;
		UI_QUESTREWARD_ITEM eFourthItem = UI_QUESTREWARD_ITEM::QUESTITEM_END;
		_int iFourthAmount = 0;

		_bool bIsEnding = false; // ������ ��� Ŭ���� �ȿ��� ��ü������ reward�� ����.

	} REWARDS_DESC;

protected:
	CUI_Quest_Reward_Item(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Quest_Reward_Item(const CUI_Quest_Reward_Item& rhs);
	virtual ~CUI_Quest_Reward_Item() = default;

public:
	virtual void Set_Active(_bool bActive) override;
	void Set_Alpha(_float fAlpha) { m_fAlpha = fAlpha; }
	void Set_Type(UI_QUESTREWARD_ITEM eType) { m_eType = eType; }
	void Set_Usable(_bool bUsable) { m_bUsable = bUsable; }
	void Set_Amount(_int iAmount) { m_iAmount = iAmount; }
	void Set_Position(_int iTotal, _int iOrder);
	void Set_EndingReward() { m_bIsEnding = true; }

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
	UI_QUESTREWARD_ITEM m_eType = { REWARD_EXP };
	_float2 m_vTextPosition = _float2(0.f, 0.f);

//	_bool m_bSetInt = { false };
//	_int m_iRandomInt = { 0 };
//	_float m_fRandomTimeAcc = { 0.f };

	_int m_iAmount = { 0 };
	_bool m_bUsable = { false };
	_bool m_bIsEnding = { false };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Quest_Reward_Item* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
