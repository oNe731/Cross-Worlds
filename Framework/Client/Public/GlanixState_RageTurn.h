#pragma once

#include "GlanixState_Base.h"

BEGIN(Client)

class CGlanixState_RageTurn final : public CGlanixState_Base
{
private:
	CGlanixState_RageTurn(CStateMachine* pStateMachine);
	virtual ~CGlanixState_RageTurn() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_vector m_vDestPos = {};
	_float  m_fTurnSpeed = 0.f;

	_bool   m_bIsOffsetJump = false;

public:
	static CGlanixState_RageTurn* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

