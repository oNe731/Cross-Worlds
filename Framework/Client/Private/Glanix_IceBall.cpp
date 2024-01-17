#include "stdafx.h"
#include "Glanix_IceBall.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"

#include "Glanix.h"
#include "Particle_Manager.h"
#include "Utils.h"

CGlanix_IceBall::CGlanix_IceBall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CMonsterProjectile(pDevice, pContext, strObjectTag)
{
}

CGlanix_IceBall::CGlanix_IceBall(const CGlanix_IceBall& rhs)
	: CMonsterProjectile(rhs)
	, m_fTime(0.f)
	, m_fDelteTime(rhs.m_fDelteTime)
{

}

HRESULT CGlanix_IceBall::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGlanix_IceBall::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	Set_Collider_Elemental(m_pOwner->Get_Stat().eElementType);
	Set_Collider_AttackMode(CCollider::ATTACK_TYPE::BLOW, 0.f, 0.f, 0.f, true);
	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);
	m_fTime = 0.f;
	m_fDelteTime = 5.f;

	return S_OK;
}

void CGlanix_IceBall::Tick(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	m_pTransformCom->Move(XMVector3Normalize(m_vInitLook), 20.f, fTimeDelta);
	m_pTransformCom->Rotation_Acc(XMVector3Normalize(m_pTransformCom->Get_Right()), XMConvertToRadians(180.f) * 4.f * fTimeDelta);

	__super::Tick(fTimeDelta);

	if (m_fTime > m_fDelteTime)
	{
		m_fTime = m_fDelteTime - m_fTime;
		Reserve_Dead(true);
		Set_Dead(true);
	}

	// EffectCreate
	if (!m_bStartBallMove)
	{
		m_bStartBallMove = true;
		m_fStartBallMatrix = m_pTransformCom->Get_WorldMatrix();
	}

	m_fStartBallMatrix.r[CTransform::STATE_POSITION] = m_pTransformCom->Get_Position();
	GET_INSTANCE(CParticle_Manager)->Tick_Generate_Particle_To_Matrix(&m_fEffectAcc, CUtils::Random_Float(0.2f, 0.4f), fTimeDelta,
		TEXT("Particle_Glanix_Skill_IceBall_Circle"), m_fStartBallMatrix, _float3(0.f, 0.f, -0.2f));
}

void CGlanix_IceBall::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CGlanix_IceBall::Render()
{
	if (nullptr == m_pShaderCom || nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	Matrix RenderMatrix = m_pTransformCom->Get_WorldMatrix();
	Vec3 vPosition = RenderMatrix.Translation();
	vPosition.y += 2.f;
	RenderMatrix.Translation(vPosition);

	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &RenderMatrix.Transpose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	_uint iPassIndex = 0;

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (m_bReserveDead)
			iPassIndex = 2;
		else if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			iPassIndex = 0;
		else
			iPassIndex++;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CGlanix_IceBall::Render_ShadowDepth()
{
	if (FAILED(__super::Render_ShadowDepth()))
		return E_FAIL;

	return S_OK;
}


void CGlanix_IceBall::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
	//if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_MONSTER &&
	//	tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
	//	tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	//{
	//	if (dynamic_cast<CGlanix*>(tInfo.pOther)->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Get_CurrState() == CGlanix::GLANIX_RAGECHARGE)
	//	{
	//		dynamic_cast<CGlanix*>(tInfo.pOther)->Set_IsCrash(true);
	//		Set_Dead(this);
	//	}
	//}
}

void CGlanix_IceBall::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
	//if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_MONSTER &&
	//	tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
	//	tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	//{
	//	if (dynamic_cast<CGlanix*>(tInfo.pOther)->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Get_CurrState() == CGlanix::GLANIX_RAGECHARGE)
	//	{
	//		dynamic_cast<CGlanix*>(tInfo.pOther)->Set_IsCrash(true);
	//		Set_Dead(this);
	//	}
	//}
}

void CGlanix_IceBall::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CGlanix_IceBall::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);
	if (m_pRigidBodyCom->Get_Velocity().y <= 0.f)
	{
		m_pRigidBodyCom->Set_Ground(true);
		m_pRigidBodyCom->Set_Use_Gravity(false);
	}
}

void CGlanix_IceBall::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
}

void CGlanix_IceBall::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
	m_pRigidBodyCom->Set_Ground(false);
	m_pRigidBodyCom->Set_Use_Gravity(true);
}

HRESULT CGlanix_IceBall::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (m_pOwner != nullptr)
	{
		/* For.Com_Transform */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
			return E_FAIL;

		m_pGlanix = dynamic_cast<CGlanix*>(m_pOwner);

		m_pTransformCom->Set_WorldMatrix(m_pGlanix->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_WorldMatrix());
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTransformCom->Get_Position());
		m_pTransformCom->Set_Scale(Vec3(5.f, 5.f, 5.f));

		m_vInitLook = XMVector3Normalize(m_pTransformCom->Get_Look());
	}
	else
	{
		MSG_BOX("Fail Create : Glanix IceBall, m_pOwner == nullptr");
		return E_FAIL;
	}


	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_GlanixIceBall"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_PhysXBody */
	CPhysX_Controller::CONTROLLER_DESC ControllerDesc;

	ControllerDesc.eType = CPhysX_Controller::CAPSULE;
	ControllerDesc.pTransform = m_pTransformCom;
	ControllerDesc.vOffset = { 0.f, 2.f, 0.f };
	ControllerDesc.fRaidus = 2.f;
	ControllerDesc.fHeight = 5.f;
	ControllerDesc.fMaxJumpHeight = 10.f;
	ControllerDesc.pOwner = this;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_PhysXController"), TEXT("Com_Controller"), (CComponent**)&m_pControllerCom, &ControllerDesc)))
		return E_FAIL;

	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	/* For. Com_RigidBody*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
		return E_FAIL;

	return S_OK;
}

#pragma region Ready_Colliders
HRESULT CGlanix_IceBall::Ready_Colliders()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 3.f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::ATTACK, &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

#pragma endregion

CGlanix_IceBall* CGlanix_IceBall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CGlanix_IceBall* pInstance = new CGlanix_IceBall(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CGlanix_IceBall");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CGlanix_IceBall::Clone(void* pArg)
{
	CGlanix_IceBall* pInstance = new CGlanix_IceBall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGlanix_IceBall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanix_IceBall::Free()
{
	__super::Free();
}

