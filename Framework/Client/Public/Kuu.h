#pragma once
#include "GameNpc.h"

BEGIN(Client)

class CKuu final : public CGameNpc
{
private:
	CKuu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CKuu(const CKuu& rhs);
	virtual ~CKuu() = default;

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

public:
	void Set_KuuTarget_Player();

	Vec4 Get_GoalPosition();
	void Set_GoalPosition();

public:
	void Set_Active(const _bool& bActive);

private:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Ready_States() override;
	virtual HRESULT Ready_Colliders() override;

private:
	void Kuu_Flying(const _float& fTimeDelta);

private:
	HRESULT Ready_Sockets();

private:
	class CCharacter* m_pPlayer = nullptr;
	class CTransform* m_pPlayerTransform = nullptr;

private:
	class CUI_World_NPCTag* m_pTag = { nullptr };
//	class CUI_World_NPCSpeechBalloon* m_pBalloon = { nullptr };

private:
	LERP_FLOAT_DESC m_tLerpDesc = {};

	_float m_fDuration = 1.f;
	_float m_fMinDestY = 0.8f;
	_float m_fMaxDestY = 1.2f;

	_float m_fY = 0.8f;
	_bool  m_bIsUp = false;

	/* Damping */
	Vec4		 m_vCurPos = Vec4::Zero;
	const _float m_fDampingCoefficient = 0.027f; /* (144 ������ ���� ���� ��� - 0�� 1���� ������ ������, ���� Ŭ���� ���� ����)*/

	_bool	m_bActive = true;

public:
	static CKuu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

