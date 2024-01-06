#include "stdafx.h"
#include "CaliaHuman.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CCaliaHuman::CCaliaHuman(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CCaliaHuman::CCaliaHuman(const CCaliaHuman& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CCaliaHuman::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCaliaHuman::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

void CCaliaHuman::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCaliaHuman::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CCaliaHuman::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CCaliaHuman::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CCaliaHuman::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CCaliaHuman::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CCaliaHuman::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { -10.f, 0.f, 0.f, 1.f });
	m_vInitPos = m_pTransformCom->Get_Position();

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_CaliaHuman"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;


	return S_OK;
}

HRESULT CCaliaHuman::Ready_States()
{
	m_tStat.fSpeed = 0.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_CaliaHuman.ao|CaliaHuman_Stand");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_CaliaHuman.ao|CaliaHuman_Talk01");
	strAnimationName.push_back(L"SKM_CaliaHuman.ao|CaliaHuman_Stand");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_CaliaHuman.ao|CaliaHuman_Walk");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_CaliaHuman.ao|CaliaHuman_Walk");
	strAnimationName.push_back(L"SKM_CaliaHuman.ao|CaliaHuman_Stand");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_TALK);

	return S_OK;
}

HRESULT CCaliaHuman::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCaliaHuman::Ready_Sockets()
{
	return S_OK;
}

CCaliaHuman* CCaliaHuman::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CCaliaHuman* pInstance = new CCaliaHuman(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CCaliaHuman");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCaliaHuman::Clone(void* pArg)
{
	CCaliaHuman* pInstance = new CCaliaHuman(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CCaliaHuman");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCaliaHuman::Free()
{
	__super::Free();
}