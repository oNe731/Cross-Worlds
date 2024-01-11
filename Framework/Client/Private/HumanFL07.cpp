#include "stdafx.h"
#include "HumanFL07.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

#include "UI_World_NPCTag.h"

CHumanFL07::CHumanFL07(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CHumanFL07::CHumanFL07(const CHumanFL07& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CHumanFL07::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanFL07::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	// UI NameTag
	CGameObject* pTag = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_NPC_Tag"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pTag)
		return E_FAIL;

	m_pTag = dynamic_cast<CUI_World_NPCTag*>(pTag);
	m_pTag->Set_Owner(this, m_strKorName, 2.2f);

	return S_OK;
}

void CHumanFL07::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
}

void CHumanFL07::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CHumanFL07::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CHumanFL07::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CHumanFL07::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CHumanFL07::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CHumanFL07::Ready_States()
{
	m_tStat.fSpeed = 0.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFL07.ao|HumanFL07_Stand");
	strAnimationName.push_back(L"SKM_HumanFL07.ao|HumanFL07_Hello01");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFL07.ao|HumanFL07_Talk01");
	strAnimationName.push_back(L"SKM_HumanFL07.ao|HumanFL07_Stand");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFL07.ao|HumanFL07_NeutralWalk01");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFL07.ao|HumanFL07_NeutralWalk01");
	strAnimationName.push_back(L"SKM_HumanFL07.ao|HumanFL07_Stand");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CHumanFL07::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanFL07::Ready_Sockets()
{
	return S_OK;
}

CHumanFL07* CHumanFL07::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHumanFL07* pInstance = new CHumanFL07(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CHumanFL07");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHumanFL07::Clone(void* pArg)
{
	CHumanFL07* pInstance = new CHumanFL07(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanFL07");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanFL07::Free()
{
	__super::Free();
	
	Safe_Release(m_pTag);
}