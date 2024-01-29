#pragma once

#include "State_CurlingGame_Base.h"

BEGIN(Client)

class CState_CurlingGame_Intro final : public CState_CurlingGame_Base
{
private:
	CState_CurlingGame_Intro(CManager_StateMachine* pStateMachine);
	virtual ~CState_CurlingGame_Intro() = default;

public:
	virtual HRESULT Initialize() override;

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(const _float& fTimeDelta) override;
	virtual void LateTick_State(const _float& fTimeDelta) override;
	virtual void Exit_State() override;
	virtual HRESULT Render() override;

public:
	static CState_CurlingGame_Intro* Create(CManager_StateMachine* pStateMachine);
	virtual void Free() override;

};

END
