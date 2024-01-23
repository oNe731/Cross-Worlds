#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_Rage1Loop_JumpStamp final : public CStelliaState_Base
{
private:
	CStelliaState_Rage1Loop_JumpStamp(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Rage1Loop_JumpStamp() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_vector vDestPos = {};

public:
	static CStelliaState_Rage1Loop_JumpStamp* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

