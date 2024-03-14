#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_Rage3Charge final : public CStelliaState_Base
{
private:
	CStelliaState_Rage3Charge(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Rage3Charge() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	void First_GuardEvent(_float fTimeDelta);
	void Player_KnockDown();
	void Check_RangeOut();

	void Create_GuardEffect();
	void Delete_GuardEffect();
	void Create_ResultEffect(_bool bSuccess);

private:
	// ���� �̺�Ʈ
	_bool m_bIsFinishGuard = false;

	// ���� �浹
	_bool m_bIsStartEvent = false;
	_bool m_bIsTimeSlep = false;
	_bool m_bIsSlow = false;

	_float m_fAccSlepTime = 0.f;
	_float m_fSlepTime = 0.f;

	// Ŭ�� �׼�
	_int m_iClickPower = 0;
	_int m_iClickDest = 0;

	_float m_fAccClickTime = 0.f;
	_float m_fLimitTime = 0.f;

	_float m_fAccDecreaseTime = 0.f;
	_float m_fDecreaseTime = 0.f;

	// Ŭ�� �׼� �� ����� ī�޶� ����ũ �ð�
	_float m_fAccShakeTime = 0.f;
	_float m_fShakeTime = 0.f;

	_int m_iBreakCount = 0;

	// �������ڸ��� �극��ũ ��°� ����
	_bool m_bIsStartPosOut = false;

	// ����Ʈ
	_bool m_bGuardEffect = false;
	class CEffect*   m_pPlayerGuard = nullptr;
	class CParticle* m_pSpark_Player = nullptr;
	class CParticle* m_pSpark_Monster = nullptr;

	_bool	m_bNoGuardCollision = false; // �÷��̾ ���尡 �ƴ� ���·� ���ڸ��ƶ� �浹

	_int iRealClickNum = 0;

	_float m_fAcc = 0.f;
	_bool m_bPlaySound = false;
public:
	static CStelliaState_Rage3Charge* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

