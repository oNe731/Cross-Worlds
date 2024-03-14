#pragma once

#include "GlanixState_Base.h"

BEGIN(Client)

class CGlanixState_IntroRoar final : public CGlanixState_Base
{
private:
	CGlanixState_IntroRoar(CStateMachine* pStateMachine);
	virtual ~CGlanixState_IntroRoar() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

public:
	static CGlanixState_IntroRoar* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

