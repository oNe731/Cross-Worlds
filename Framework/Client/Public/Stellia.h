#pragma once
#include "Boss.h"

BEGIN(Client)
class CStellia final : public CBoss
{
public:
	enum STELLIA_STATE {
		STELLIA_SPAWN,
		STELLIA_COMBATIDLE,
		STELLIA_CHASE,
		STELLIA_ATTACK1, STELLIA_ATTACK2, STELLIA_SPINTAIL, STELLIA_JUMPSTAMP, STELLIA_LASER,
		STELLIA_TRIPLELASER, STELLIA_CHARGE, STELLIA_BIGBANG,
		STELLIA_CHASEJUMPSTAMP,
		STELLIA_COUNTERSTART, STELLIA_COUNTERLOOP, STELLIA_COUNTEREND,
		STELLIA_BERSERK,
		/* ������ 1*/
		STELLIA_RAGE1START_TURN_OC, STELLIA_RAGE1START_JUMP, STELLIA_RAGE1START_TURN_OL, STELLIA_RAGE1START,
		STELLIA_RAGE1LOOP_TURN, STELLIA_RAGE1LOOP_IDLE, STELLIA_RAGE1LOOP_EXPLOSION,
		STELLIA_RAGE1LOOP_SPINTAIL, STELLIA_RAGE1LOOP_JUMPSTAMP,

		/* ������ 2*/
		STELLIA_RAGE2START_TURN_OC, STELLIA_RAGE2START_JUMP, STELLIA_RAGE2START_TURN_OL, STELLIA_RAGE2START,
		STELLIA_RAGE2LOOP, STELLIA_RAGE2FINISH,

		/* ������ 3*/

		STELLIA_TURN,
		STELLIA_DEAD,
		STELLIA_END
	};

private:
	CStellia(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat);
	CStellia(const CStellia& rhs);
	virtual ~CStellia() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

public:
	virtual void On_Damaged(const COLLISION_INFO& tInfo) override;
	virtual void Set_SkillTree() override;

	/* ������2 ���� */
	void Clear_CrystalBingoCount() { m_iCrystalBingoCount = 0; }

	void Set_CrystalBingoCount(_int iCount) { m_iCrystalBingoCount += iCount; }
	void Set_CrystalSuccessCount(_int iCount) { m_iCrystalSuccessCount += iCount; }
	void Set_CrystalFailCount(_int iCount) { m_iCrystalFailCount += iCount; }
	_int Get_CrystalBingoCount() { return m_iCrystalBingoCount; }
	_int Get_CrystalSuccessCount() { return m_iCrystalSuccessCount; }
	_int Get_CrystalFailCount() { return m_iCrystalFailCount; }

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_States();
	virtual HRESULT Ready_Colliders();

private:
	HRESULT Ready_Sockets();
	HRESULT Ready_Parts();

public:
	// ������ 1
	_int Get_AccDamage() { return m_iAccDamage; }
	_int Get_DestDamage() { return m_iDestDamage; }
	void Reset_RageAccDamage() { m_iAccDamage = 0; }

	// ������ 2
	HRESULT Create_Crystals();
	HRESULT Clear_Crystals();

	class CStellia_Crystal_Controller* Get_CrystalController() const { return m_pCrystalController; }
	void Set_CrystalTurnData();

	// ���ڸ��� ���� 
	_bool Get_IsStelliaHit() { return m_bIsStelliaHit; }
	void  Set_StelliaHit(_bool bIsStelliaHit) { m_bIsStelliaHit = bIsStelliaHit; }

private:
	// ������ 1
	_int m_iAccDamage = 0;
	_int m_iDestDamage = 0;

	// ������ 2
	_int m_iCrystalBingoCount = 0;
	_int m_iCrystalSuccessCount = 0;
	_int m_iCrystalFailCount = 0;

	class CStellia_Crystal_Controller* m_pCrystalController = nullptr;

	// ���ڸ��� ���� 
	_bool m_bIsStelliaHit = true;

public:
	static CStellia* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

