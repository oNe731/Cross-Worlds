#pragma once

#include "Client_Defines.h"
#include "State.h"

#include "GameObject.h"
#include "Model.h"
#include "Transform.h"
#include "StateMachine.h"
#include "Navigation.h"
#include "RigidBody.h"

#include "Character_Dummy.h" // �Ŀ� �÷��̾�� ��ü

BEGIN(Client)

class CDMWitch;
class CDMWitchState_Base abstract : public CState
{
protected:
	CDMWitchState_Base(CStateMachine* pStateMachine);
	virtual ~CDMWitchState_Base() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) PURE;
	virtual void Tick_State(_float fTimeDelta) PURE;
	virtual void Exit_State() PURE;

public:
	void	Init_Pattern();

protected:
	virtual _bool State_Wait(_float fDestTime, _float fTimeDelta);

protected:
	CCharacter_Dummy* m_pPlayer = nullptr; // �Ŀ� �÷��̾�� ��ü.
	CTransform* m_pPlayerTransform = nullptr;

	CDMWitch* m_pWitch = nullptr;

	_float m_fRunSpeed = 0.f;

	/* ���� ���� ���� ������ */
	vector<_uint> m_vecAtkState;
	static _uint  m_iAtkIndex;

private:
	_float m_fTime = 0.f;

public:
	virtual void Free() override;
};

END
