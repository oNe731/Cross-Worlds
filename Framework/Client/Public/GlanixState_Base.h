#pragma once

#include "Client_Defines.h"
#include "State.h"

#include "GameObject.h"
#include "Model.h"
#include "Transform.h"
#include "StateMachine.h"
#include "Navigation.h"
#include "RigidBody.h"

#include "Character_Dummy.h" // 후에 플레이어로 교체

BEGIN(Client)

class CGlanix;
class CGlanixState_Base abstract : public CState
{
protected:
	CGlanixState_Base(CStateMachine* pStateMachine);
	virtual ~CGlanixState_Base() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) PURE;
	virtual void Tick_State(_float fTimeDelta) PURE;
	virtual void Exit_State() PURE;

protected:
	virtual _bool State_Wait(_float fDestTime, _float fTimeDelta);

protected:
	CCharacter_Dummy* m_pPlayer = nullptr; // 후에 플레이어로 교체.
	CTransform* m_pPlayerTransform = nullptr;
	
	CGlanix* m_pGlanix = nullptr;

	_float m_fRunSpeed = 0.f;

	/* 공격 패턴 위한 변수들 */
	vector<_uint> m_vecAtkState;
	static _uint  m_iAtkIndex;

private:
	_float m_fTime = 0.f;

public:
	virtual void Free() override;
};

END
