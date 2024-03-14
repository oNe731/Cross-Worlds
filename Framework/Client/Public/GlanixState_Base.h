#pragma once

#include "Client_Defines.h"
#include "State.h"

#include "GameObject.h"
#include "Model.h"
#include "Transform.h"
#include "StateMachine.h"
#include "Navigation.h"
#include "RigidBody.h"

#include "Character.h" // �Ŀ� �÷��̾�� ��ü

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

public:
	void	Init_Pattern();

protected:
	_bool State_Wait(_float fDestTime, _float fTimeDelta);
	void  Start_Pattern();

protected:
	CCharacter* m_pPlayer = nullptr; // �Ŀ� �÷��̾�� ��ü.
	CTransform* m_pPlayerTransform = nullptr;
	
	CGlanix* m_pGlanix = nullptr;

	_float m_fRunSpeed = 0.f;

	/* ���� ���� ���� ������ */
	vector<_uint> m_vecAtkState;
	static _uint  m_iAtkIndex;

protected:
	MOTION_TRAIL_DESC m_MotionTrailDesc = {};

private:
	_float m_fTime = 0.f;

public:
	virtual void Free() override;
};

END
