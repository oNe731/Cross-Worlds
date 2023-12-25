#pragma once
#include "Client_Defines.h"
#include "State_Character.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_SwordMan_Skill_SipohoningLunge : public CState_Character
{

private:
	CState_SwordMan_Skill_SipohoningLunge(class CStateMachine* pMachine);
	~CState_SwordMan_Skill_SipohoningLunge() = default;


public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);


public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

public:
	static CState_SwordMan_Skill_SipohoningLunge* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};

END