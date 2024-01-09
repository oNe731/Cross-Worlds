#include "stdafx.h"
#include "TreeGrandfa.h"

#include "GameInstance.h"

CTreeGrandfa::CTreeGrandfa(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CTreeGrandfa::CTreeGrandfa(const CTreeGrandfa& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CTreeGrandfa::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTreeGrandfa::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	m_pModelCom->Set_Animation(TEXT("SKM_TreeGrandfa.ao|TreeGrandfa_Stand"));

	return S_OK;
}

void CTreeGrandfa::Tick(_float fTimeDelta)
{
	if (KEY_TAP(KEY::J))
	{
		_uint iCurAnimIndex = m_pModelCom->Get_CurrAnimationIndex();
		m_pModelCom->Set_Animation(iCurAnimIndex + 1);
	}
	else if (KEY_TAP(KEY::K))
	{
		_int iCurAnimIndex = m_pModelCom->Get_CurrAnimationIndex() - 1;
		if (iCurAnimIndex < 0)
			iCurAnimIndex = 0;
		m_pModelCom->Set_Animation(iCurAnimIndex);
	}

	m_pStateCom->Tick_State(fTimeDelta);

	m_pRigidBodyCom->Update_RigidBody(fTimeDelta);
	m_pControllerCom->Tick_Controller(fTimeDelta);
	__super::Tick(fTimeDelta);
}

void CTreeGrandfa::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

HRESULT CTreeGrandfa::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CTreeGrandfa::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CTreeGrandfa::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CTreeGrandfa::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CTreeGrandfa::On_Damaged(const COLLISION_INFO& tInfo)
{
}


HRESULT CTreeGrandfa::Ready_States()
{

	return S_OK;
}

HRESULT CTreeGrandfa::Ready_Colliders()
{
	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 50.f, 70.f, 50.f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	OBBDesc.vOffsetPosition = Vec3(0.f, 70.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::BODY, &OBBDesc)))
		return E_FAIL;

	OBBBox.Extents = { 100.f, 100.f, 50.f };
	OBBDesc.vOffsetPosition = Vec3(0.f, 70.f, -100.f);
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::ATTACK, &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTreeGrandfa::Ready_Sockets()
{
	return S_OK;
}

CTreeGrandfa* CTreeGrandfa::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CTreeGrandfa* pInstance = new CTreeGrandfa(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CTreeGrandfa");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTreeGrandfa::Clone(void* pArg)
{
	CTreeGrandfa* pInstance = new CTreeGrandfa(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CTreeGrandfa");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTreeGrandfa::Free()
{
	__super::Free();
}