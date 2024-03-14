#include "stdafx.h"
#include "HumanFL05.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

#include "UI_World_NPCTag.h"
#include "UI_World_NPCSpeechBalloon.h"

CHumanFL05::CHumanFL05(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CHumanFL05::CHumanFL05(const CHumanFL05& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CHumanFL05::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanFL05::Initialize(void* pArg)
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
	m_pTag->Set_Owner(this, m_strKorName, 1.8f);

	CGameObject* pBalloon = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_NPC_SpeechBalloon"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pBalloon)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_World_NPCSpeechBalloon*>(pBalloon))
		return E_FAIL;
	m_pBalloon = dynamic_cast<CUI_World_NPCSpeechBalloon*>(pBalloon);
	m_pBalloon->Set_Owner(this, 2.f);
	m_pBalloon->Set_Balloon(TEXT("ȣȣȣ ���� ������ �� ����"));

	return S_OK;
}

void CHumanFL05::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
	if (nullptr != m_pBalloon)
		m_pBalloon->Tick(fTimeDelta);
}

void CHumanFL05::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->LateTick(fTimeDelta);
	if (nullptr != m_pBalloon)
		m_pBalloon->LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CHumanFL05::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CHumanFL05::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CHumanFL05::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CHumanFL05::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CHumanFL05::Ready_States()
{
	m_tStat.fSpeed = 0.5f;
	m_tStat.bHasMove = false;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFL05.ao|HumanFL05_Stand");
	strAnimationName.push_back(L"SKM_HumanFL05.ao|HumanFL05_Idle01");
	strAnimationName.push_back(L"SKM_HumanFL05.ao|HumanFL05_Idle02");
	strAnimationName.push_back(L"SKM_HumanFL05.ao|HumanFL05_Idle03");
	strAnimationName.push_back(L"SKM_HumanFL05.ao|HumanFL05_CSGuideMissionIntro01");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFL05.ao|HumanFL05_Talk01");
	strAnimationName.push_back(L"SKM_HumanFL05.ao|HumanFL05_Stand");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFL05.ao|HumanFL05_InstantTurn");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFL05.ao|HumanFL05_InstantTurn");
	strAnimationName.push_back(L"SKM_HumanFL05.ao|HumanFL05_Stand");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CHumanFL05::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanFL05::Ready_Sockets()
{
	return S_OK;
}

CHumanFL05* CHumanFL05::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHumanFL05* pInstance = new CHumanFL05(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CHumanFL05");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHumanFL05::Clone(void* pArg)
{
	CHumanFL05* pInstance = new CHumanFL05(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanFL05");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanFL05::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
	Safe_Release(m_pBalloon);
}