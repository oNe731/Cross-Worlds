#pragma once

#include "GlanixState_Base.h"
BEGIN(Client)

class CGlanixState_QuadBlow final : public CGlanixState_Base
{
private:
	CGlanixState_QuadBlow(CStateMachine* pStateMachine);
	virtual ~CGlanixState_QuadBlow() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float m_fFowardSpeed = 0.f;

public:
	static CGlanixState_QuadBlow* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

