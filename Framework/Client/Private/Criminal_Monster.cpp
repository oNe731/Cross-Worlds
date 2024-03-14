#include "stdafx.h"
#include "Criminal_Monster.h"

#include "GameInstance.h"

#include "Criminal_MonsterBT.h"
#include "UI_MonsterHP_World.h"
#include "UIDamage_Manager.h"
#include "UI_Minimap_Icon.h"
#include "UI_Manager.h"

#include "Quest_Manager.h"

CCriminal_Monster::CCriminal_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
	: CMonster(pDevice, pContext, strObjectTag, tStat)
{
}

CCriminal_Monster::CCriminal_Monster(const CCriminal_Monster& rhs)
	: CMonster(rhs)
{
}

HRESULT CCriminal_Monster::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CCriminal_Monster::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	
	if (pArg != nullptr)
	{
		Vec4* pInitializePosition = (Vec4*)pArg;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(*pInitializePosition, 1.f));
	}

	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	if (nullptr == m_pControllerCom)
		return E_FAIL;

	m_pControllerCom->Set_EnterLevel_Position(m_pTransformCom->Get_Position());

	if (FAILED(__super::Ready_RoamingPoint()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	m_pModelCom->Set_Animation(0);

	if (FAILED(Ready_States()))
		return E_FAIL;

	CGameObject* pHPBar = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Monster_WorldHPBar"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pHPBar)
		return E_FAIL;
	m_pHPBar = dynamic_cast<CUI_MonsterHP_World*>(pHPBar);
	m_pHPBar->Set_Owner(this, m_tStat.eElementType, 2.f);

	m_vBloomPower = _float3(0.8f, 0.8f, 0.8f);

	m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT] = true;

	//m_pMinimapIcon
	CGameObject* pIcon = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Minimap_Icon"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pIcon)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_Minimap_Icon*>(pIcon))
		return E_FAIL;
	m_pMinimapIcon = dynamic_cast<CUI_Minimap_Icon*>(pIcon);
	m_pMinimapIcon->Set_Owner(this);

	return S_OK;
}

void CCriminal_Monster::Tick(_float fTimeDelta)
{
	if (nullptr != m_pHPBar)
		m_pHPBar->Tick(fTimeDelta);

	if (nullptr != m_pMinimapIcon)
	{
		if (true == CUI_Manager::GetInstance()->Is_InMinimap(m_pTransformCom))
			m_pMinimapIcon->Set_Active(true);
		else
			m_pMinimapIcon->Set_Active(false);
	}

	if (nullptr != m_pMinimapIcon)
		m_pMinimapIcon->Tick(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CCriminal_Monster::LateTick(_float fTimeDelta)
{
	if (nullptr != m_pHPBar)
		m_pHPBar->LateTick(fTimeDelta);

	if (nullptr != m_pMinimapIcon)
		m_pMinimapIcon->LateTick(fTimeDelta);

	__super::LateTick(fTimeDelta);
}

HRESULT CCriminal_Monster::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CCriminal_Monster::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

void CCriminal_Monster::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);

	/* �ǰ� */
	if (m_tStat.fHp > 0.f)
	{
		if ((tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER_PROJECTILE) &&
			tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK)
		{
			if (tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
			{
				m_pTransformCom->LookAt_ForLandObject(dynamic_cast<CTransform*>(tInfo.pOther->Get_Component<CTransform>(TEXT("Com_Transform")))->Get_Position());

				/* Blow */
				if (tInfo.pOtherCollider->Get_AttackType() == CCollider::ATTACK_TYPE::BLOW)
				{
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD] = true;

					On_Damaged(tInfo);

					m_pModelCom->Set_Animation(TEXT("SKM_BlackCircleAgent.ao|Air"));

					m_pRigidBodyCom->Add_Velocity(-m_pTransformCom->Get_Look(), m_tStat.fAirVelocity, false);
					m_pRigidBodyCom->Add_Velocity({ 0.f, 1.f, 0.f, 1.f }, m_tStat.fAirVelocity / 1.5f, false);

					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_BLOW] = true;
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_AIR] = false;
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT] = true;
				}

				/* Air || Bound */
				else if (tInfo.pOtherCollider->Get_AttackType() == CCollider::ATTACK_TYPE::AIR_BORNE ||
					tInfo.pOtherCollider->Get_AttackType() == CCollider::ATTACK_TYPE::BOUND)
				{
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD] = true;

					On_Damaged(tInfo);

					m_pModelCom->Set_Animation(TEXT("SKM_BlackCircleAgent.ao|Air"));
					m_pRigidBodyCom->Add_Velocity({ 0.f, 1.f, 0.f, 1.f }, m_tStat.fAirVelocity / 2.f, false);

					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_BLOW] = false;
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_AIR] = true;
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT] = true;
				}

				/* Stun */
				else if (tInfo.pOtherCollider->Get_AttackType() == CCollider::ATTACK_TYPE::STUN)
				{
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD] = false;

					On_Damaged(tInfo);

					if (!m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_BLOW] && !m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_AIR])
					{

						if (m_pModelCom->Get_CurrAnimation()->Get_AnimationName() != TEXT("SKM_BlackCircleAgent.ao|Idle"))
							m_pModelCom->Set_Animation(TEXT("SKM_BlackCircleAgent.ao|Idle"));

						m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_STUN] = true;
					}

					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT] = true;
				}

				/* Hit */
				else if (tInfo.pOtherCollider->Get_AttackType() == CCollider::ATTACK_TYPE::STRONG ||
					tInfo.pOtherCollider->Get_AttackType() == CCollider::ATTACK_TYPE::WEAK)
				{
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD] = false;

					On_Damaged(tInfo);

					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_WEAK] = true;
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT] = true;
				}
			}
		}
	}

}

void CCriminal_Monster::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CCriminal_Monster::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CCriminal_Monster::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);
}

void CCriminal_Monster::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
}

void CCriminal_Monster::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
}

HRESULT CCriminal_Monster::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For. Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Criminal"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_PhysXBody */
	CPhysX_Controller::CONTROLLER_DESC ControllerDesc;

	ControllerDesc.eType = CPhysX_Controller::CAPSULE;
	ControllerDesc.pTransform = m_pTransformCom;
	ControllerDesc.vOffset = { 0.f, 1.125f, 0.f };
	ControllerDesc.fHeight = 5.f;
	ControllerDesc.fMaxJumpHeight = 10.f;
	ControllerDesc.fRaidus = .2f;
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

HRESULT CCriminal_Monster::Ready_States()
{
	m_pBTCom = CCriminal_MonsterBT::Create(m_pDevice, m_pContext, this);

	strKorName = TEXT("���� ����");
	strSubName = TEXT("���� ���ڵ�");
	m_tStat.eElementType = ELEMENTAL_TYPE::WOOD;

	m_tStat.iLv = 1;
	m_tStat.fMaxHp = 10000;
	m_tStat.fHp = 10000;
	m_tStat.iAtk = 25;
	m_tStat.iDef = 0;

	m_tStat.fAirVelocity = 6.5f;
	m_tStat.fAirDeadVelocity = 9.f;

	return S_OK;
}

HRESULT CCriminal_Monster::Ready_Colliders()
{
	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 50.f, 100.f, 30.f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	OBBDesc.vOffsetPosition = Vec3(0.f, 100.f, 0.f);

	/* Body */
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::BODY, &OBBDesc)))
		return E_FAIL;

	/* Atk */
	OBBBox.Extents = { 0.f, 0.f, 0.f };
	OBBDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::ATTACK, &OBBDesc)))
		return E_FAIL;

	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 1.f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 100.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BOUNDARY, &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

CCriminal_Monster* CCriminal_Monster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
{
	CCriminal_Monster* pInstance = new CCriminal_Monster(pDevice, pContext, strObjectTag, tStat);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Craete : CCriminal_Monster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCriminal_Monster::Clone(void* pArg)
{
	CCriminal_Monster* pInstance = new CCriminal_Monster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CCriminal_Monster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCriminal_Monster::Free()
{
	__super::Free();

	Safe_Release(m_pMinimapIcon);
	Safe_Release(m_pHPBar);
}
