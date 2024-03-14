#include "stdafx.h"
#include "GrimalkinOld01.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

#include "UI_World_NPCTag.h"
#include "UI_World_NPCSpeechBalloon.h"

CGrimalkinOld01::CGrimalkinOld01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CGrimalkinOld01::CGrimalkinOld01(const CGrimalkinOld01& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CGrimalkinOld01::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrimalkinOld01::Initialize(void* pArg)
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
	m_pTag->Set_Owner(this, m_strKorName, 1.5f);

	CGameObject* pBalloon = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_NPC_SpeechBalloon"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pBalloon)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_World_NPCSpeechBalloon*>(pBalloon))
		return E_FAIL;
	m_pBalloon = dynamic_cast<CUI_World_NPCSpeechBalloon*>(pBalloon);
	m_pBalloon->Set_Owner(this, 1.7f);
	_int iRandom = GI->RandomInt(0, 1);
	switch (iRandom)
	{
	case 0:
		m_pBalloon->Set_Balloon(TEXT("���� �ϴ��� ����"));
		break;

	case 1:
		m_pBalloon->Set_Balloon(TEXT("���̰� �㸮��."));
		break;
	}

	return S_OK;
}

void CGrimalkinOld01::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
	if (nullptr != m_pBalloon)
		m_pBalloon->Tick(fTimeDelta);
}

void CGrimalkinOld01::LateTick(_float fTimeDelta)
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

void CGrimalkinOld01::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CGrimalkinOld01::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CGrimalkinOld01::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CGrimalkinOld01::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CGrimalkinOld01::Ready_States()
{
	m_tStat.fSpeed = 1.f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"Stand01");
	strAnimationName.push_back(L"Stand03Idle01");
	strAnimationName.push_back(L"Stand02Idle01");
	strAnimationName.push_back(L"Stand01Idle01");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"Stand03Idle01");
	strAnimationName.push_back(L"Stand01");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"Walk02");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"Walk02");
	strAnimationName.push_back(L"Stand02Idle01");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CGrimalkinOld01::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrimalkinOld01::Ready_Sockets()
{
	return S_OK;
}

CGrimalkinOld01* CGrimalkinOld01::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CGrimalkinOld01* pInstance = new CGrimalkinOld01(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CGrimalkinOld01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGrimalkinOld01::Clone(void* pArg)
{
	CGrimalkinOld01* pInstance = new CGrimalkinOld01(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CGrimalkinOld01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGrimalkinOld01::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
	Safe_Release(m_pBalloon);
}
