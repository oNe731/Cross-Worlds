#pragma once
#include "Client_Defines.h"
#include "State_Character.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_Character_Neutral_Crouch_Idle : public CState_Character
{

private:
	CState_Character_Neutral_Crouch_Idle(class CStateMachine* pMachine);
	~CState_Character_Neutral_Crouch_Idle() = default;


public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);


public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	_float m_fAccIdleMotion = 0.f;
	_float m_fIdleMotionTime = 5.f;

public:
	static CState_Character_Neutral_Crouch_Idle* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};

END