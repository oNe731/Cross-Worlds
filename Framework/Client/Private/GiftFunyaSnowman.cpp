#include "stdafx.h"
#include "GiftFunyaSnowman.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CGiftFunyaSnowman::CGiftFunyaSnowman(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CGiftFunyaSnowman::CGiftFunyaSnowman(const CGiftFunyaSnowman& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CGiftFunyaSnowman::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGiftFunyaSnowman::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

void CGiftFunyaSnowman::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CGiftFunyaSnowman::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CGiftFunyaSnowman::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CGiftFunyaSnowman::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CGiftFunyaSnowman::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CGiftFunyaSnowman::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CGiftFunyaSnowman::Ready_States()
{
	m_tStat.fSpeed = 0.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	// SKM_RunnerCat.ao|RunnerCat_Stand
	strAnimationName.push_back(L"SKM_GiftFunyaSnowman.ao|FunyaSnowman_Stand01");
	strAnimationName.push_back(L"SKM_GiftFunyaSnowman.ao|FunyaSnowman_Idle01");
	strAnimationName.push_back(L"SKM_GiftFunyaSnowman.ao|FunyaSnowman_CSGiftFromSnowman01");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_GiftFunyaSnowman.ao|FunyaSnowman_Stand01Talk01");
	strAnimationName.push_back(L"SKM_GiftFunyaSnowman.ao|FunyaSnowman_Stand01");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_GiftFunyaSnowman.ao|FunyaSnowman_Walk");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_GiftFunyaSnowman.ao|FunyaSnowman_Walk");
	strAnimationName.push_back(L"SKM_GiftFunyaSnowman.ao|FunyaSnowman_Stand01");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;

}

HRESULT CGiftFunyaSnowman::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGiftFunyaSnowman::Ready_Sockets()
{
	return S_OK;
}

CGiftFunyaSnowman* CGiftFunyaSnowman::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CGiftFunyaSnowman* pInstance = new CGiftFunyaSnowman(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CGiftFunyaSnowman");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGiftFunyaSnowman::Clone(void* pArg)
{
	CGiftFunyaSnowman* pInstance = new CGiftFunyaSnowman(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CGiftFunyaSnowman");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGiftFunyaSnowman::Free()
{
	__super::Free();
}