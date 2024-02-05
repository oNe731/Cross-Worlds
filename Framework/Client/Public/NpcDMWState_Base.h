#pragma once

#include "Client_Defines.h"
#include "State.h"

#include "GameObject.h"
#include "Model.h"
#include "Transform.h"
#include "StateMachine.h"
#include "Navigation.h"
#include "RigidBody.h"

#include "Stellia.h"
#include "Character.h" 

BEGIN(Client)

class CDreamMazeWitch_Npc;
class CNpcDMWState_Base abstract : public CState
{
protected:
	CNpcDMWState_Base(CStateMachine* pStateMachine);
	virtual ~CNpcDMWState_Base() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) PURE;
	virtual void Tick_State(_float fTimeDelta) PURE;
	virtual void Exit_State() PURE;

protected:
	virtual void Following_Stellia(Vec4 vStelliaPos, const _float& fTimeDelta);

protected:
	CCharacter* m_pPlayer = nullptr;
	CTransform* m_pPlayerTransform = nullptr;
	CGameObject* m_pStellia = nullptr;

	CDreamMazeWitch_Npc* m_pWitch = nullptr;

	_float m_fFollowingSpeed = 0.f;

	/* Damping */
	Vec4		 m_vCurPos = Vec4::Zero;
	const _float m_fDampingCoefficient = 0.027f; /* (144 ������ ���� ���� ��� - 0�� 1���� ������ ������, ���� Ŭ���� ���� ����)*/

	/* ���� ���� ���� ������ */
	vector<_uint> m_vecAtkState;
	static _uint  m_iAtkIndex;


private:
	_float m_fTime = 0.f;

public:
	virtual void Free() override;
};

END
