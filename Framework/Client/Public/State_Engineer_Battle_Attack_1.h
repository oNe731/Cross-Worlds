#pragma once
#include "Client_Defines.h"
#include "State_Character.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_Engineer_Battle_Attack_1 : public CState_Character
{

private:
	CState_Engineer_Battle_Attack_1(class CStateMachine* pMachine);
	~CState_Engineer_Battle_Attack_1() = default;


public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);


public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	void Input(_float fTimeDelta);
	void Shoot();

private:
	_uint m_iShootCount = 1;

public:
	static CState_Engineer_Battle_Attack_1* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};

END