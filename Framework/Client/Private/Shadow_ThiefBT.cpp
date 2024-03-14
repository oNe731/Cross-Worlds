#include "stdafx.h"
#include "Shadow_ThiefBT.h"

#include "GameInstance.h"

#include "Shadow_Thief.h"

#include "BTNode_Condition.h"
#include "BTNode_Leaf.h"
#include "BTNode_Select.h"
#include "BTNode_RandomSelect.h"
#include "BTNode_Sequence.h"
#include "BTNode_Loop.h"

#include "Shadow_ThiefNode_Dead.h"

#include "Shadow_ThiefNode_Blow.h"
#include "Shadow_ThiefNode_Air.h"
#include "Shadow_ThiefNode_Stun.h"
#include "Shadow_ThiefNode_Hit.h"

#include "Shadow_ThiefNode_Chase.h"

#include "Shadow_ThiefNode_Attack1.h"
#include "Shadow_ThiefNode_Attack2.h"
#include "Shadow_ThiefNode_Skill1.h"

#include "Shadow_ThiefNode_Return.h"

#include "Shadow_ThiefNode_Roaming.h"
#include "Shadow_ThiefNode_Idle.h"

#include "Game_Manager.h"
#include "Player.h"

CShadow_ThiefBT::CShadow_ThiefBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonsterBT(pDevice, pContext)
{
}

CShadow_ThiefBT::CShadow_ThiefBT(const CShadow_ThiefBT& rhs)
	: CMonsterBT(rhs)
{
}

HRESULT CShadow_ThiefBT::Initialize_Prototype(CMonster* pOwner)
{
	__super::Initialize_Prototype(pOwner);

	m_pShadow_Thief = dynamic_cast<CShadow_Thief*>(m_tBTMonsterDesc.pOwner);
	m_pRootNode = CBTNode_Select::Create(this);

	/* ���� Sequence ���� */
	CBTNode_Sequence* pSeq_Dead = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Hit = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Combat = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Chase = CBTNode_Sequence::Create(this);
	//CBTNode_Sequence* pSeq_Return = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Idle = CBTNode_Sequence::Create(this);

	/* Dead ���� */
	CShadow_ThiefNode_Dead* pDeadNode = CShadow_ThiefNode_Dead::Create(&m_tBTMonsterDesc, this);

	/* Hit ���� */
	CBTNode_Select* pSel_Hit = CBTNode_Select::Create(this);
	CShadow_ThiefNode_Blow* pBlowNode = CShadow_ThiefNode_Blow::Create(&m_tBTMonsterDesc, this);
	CShadow_ThiefNode_Air* pAirNode = CShadow_ThiefNode_Air::Create(&m_tBTMonsterDesc, this);
	CShadow_ThiefNode_Stun* pStunNode = CShadow_ThiefNode_Stun::Create(&m_tBTMonsterDesc, this);
	CShadow_ThiefNode_Hit* pHitNode = CShadow_ThiefNode_Hit::Create(&m_tBTMonsterDesc, this);

	/* Combat ���� */
	CBTNode_Sequence* pSeq_Pattern = CBTNode_Sequence::Create(this);
	CShadow_ThiefNode_Attack1* pAtk1Node = CShadow_ThiefNode_Attack1::Create(&m_tBTMonsterDesc, this);
	CShadow_ThiefNode_Attack2* pAtk2Node = CShadow_ThiefNode_Attack2::Create(&m_tBTMonsterDesc, this);
	CShadow_ThiefNode_Skill1* pSkill1Node = CShadow_ThiefNode_Skill1::Create(&m_tBTMonsterDesc, this);

	/* Chase ���� */
	CShadow_ThiefNode_Chase* pChaseNode = CShadow_ThiefNode_Chase::Create(&m_tBTMonsterDesc, this);

	/* Return ���� */
	//CShadow_ThiefNode_Return* pReturnNode = CShadow_ThiefNode_Return::Create(&m_tBTMonsterDesc, this);

	/* Idel ���� */
	vector<wstring> vecAnimationName;
	vecAnimationName.push_back(TEXT("SKM_ShadowThief.ao|ShadowThief_Idle02"));
	vecAnimationName.push_back(TEXT("SKM_ShadowThief.ao|ShadowThief_Idle01"));
	vecAnimationName.push_back(TEXT("SKM_ShadowThief.ao|ShadowThief_Idle02"));
	CShadow_ThiefNode_Idle* pIdleNode = CShadow_ThiefNode_Idle::Create(&m_tBTMonsterDesc, this, vecAnimationName);
	CShadow_ThiefNode_Roaming* pRoamingNode = CShadow_ThiefNode_Roaming::Create(&m_tBTMonsterDesc, this, dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_RoamingArea());

	/* Condition ����*/
	/* function<_bool()>�� �޴� CBTNode_Condition::Create �Լ������� ��� �Լ��� ����ϰ� �ֱ� ������ �߰����� ó���� �ʿ� */
	CBTNode_Condition* pCon_IsDead = CBTNode_Condition::Create(bind(&CShadow_ThiefBT::IsZeroHp, this), pDeadNode, pAtk1Node);
	CBTNode_Condition* pCon_IsHit = CBTNode_Condition::Create(bind(&CShadow_ThiefBT::IsHit, this), pHitNode, pChaseNode);
	CBTNode_Condition* pCon_IsCombat = CBTNode_Condition::Create(bind(&CShadow_ThiefBT::IsAtk, this), nullptr, pChaseNode);
	CBTNode_Condition* pCon_IsChase = CBTNode_Condition::Create(bind(&CShadow_ThiefBT::IsChase, this), pChaseNode, pRoamingNode);
	//CBTNode_Condition* pCon_IsReturn = CBTNode_Condition::Create(bind(&CShadow_ThiefBT::IsReturn, this), pReturnNode, pIdleNode);


	m_pRootNode->Add_ChildNode(pSeq_Dead);
	pSeq_Dead->Add_ChildNode(pCon_IsDead);
	pSeq_Dead->Add_ChildNode(pDeadNode);

	m_pRootNode->Add_ChildNode(pSeq_Hit);
	pSeq_Hit->Add_ChildNode(pCon_IsHit);
	pSeq_Hit->Add_ChildNode(pSel_Hit);
	pSel_Hit->Add_ChildNode(pBlowNode);
	pSel_Hit->Add_ChildNode(pAirNode);
	pSel_Hit->Add_ChildNode(pStunNode);
	pSel_Hit->Add_ChildNode(pHitNode);

	m_pRootNode->Add_ChildNode(pSeq_Combat);
	pSeq_Combat->Add_ChildNode(pCon_IsCombat);
	pSeq_Combat->Add_ChildNode(pSeq_Pattern);
	pSeq_Pattern->Add_ChildNode(pAtk1Node);
	pSeq_Pattern->Add_ChildNode(pAtk2Node);
	pSeq_Pattern->Add_ChildNode(pSkill1Node);

	m_pRootNode->Add_ChildNode(pSeq_Chase);
	pSeq_Chase->Add_ChildNode(pCon_IsChase);
	pSeq_Chase->Add_ChildNode(pChaseNode);

	//m_pRootNode->Add_ChildNode(pSeq_Return);
	//pSeq_Return->Add_ChildNode(pCon_IsReturn);
	//pSeq_Return->Add_ChildNode(pReturnNode);

	m_pRootNode->Add_ChildNode(pSeq_Idle);
	pSeq_Idle->Add_ChildNode(pRoamingNode);
	pSeq_Idle->Add_ChildNode(pIdleNode);

	return S_OK;
}

HRESULT CShadow_ThiefBT::Initialize(void* pArg)
{
	return S_OK;
}

void CShadow_ThiefBT::Tick(const _float& fTimeDelta)
{
	if (m_tBTMonsterDesc.pOwner != nullptr)
		m_pRootNode->Tick(fTimeDelta);
}

void CShadow_ThiefBT::LateTick(const _float& fTimeDelta)
{
	//if (KEY_TAP(KEY::M))
	//{
	//	m_pRootNode->Init_Start();
	//	m_pShadow_Thief->Set_StunTime(3.f);
	//	m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_ShadowThief.ao|ShadowThief_Stun"));
	//	m_pShadow_Thief->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT, true);
	//	m_pShadow_Thief->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN, true);
	//	m_pShadow_Thief->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT, true);
	//}
}

void CShadow_ThiefBT::Init_NodeStart()
{
	m_pRootNode->Init_Start();
}

_bool CShadow_ThiefBT::IsZeroHp()
{
	if (m_pShadow_Thief->Get_Stat().fHp <= 0)
		return true;

	return false;
}

_bool CShadow_ThiefBT::IsHit()
{
	if (m_pShadow_Thief->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT))
		return true;

	return false;
}

_bool CShadow_ThiefBT::IsAtk()
{
	if (m_pShadow_Thief->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT))
	{
		if (m_pShadow_Thief->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND) ||
			m_pShadow_Thief->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK) ||
			m_pShadow_Thief->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBATIDLE))
		{
			return true;
		}
	}

	return false;
}

_bool CShadow_ThiefBT::IsChase()
{
	if (m_pShadow_Thief->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT) &&
		!m_pShadow_Thief->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK) &&
		!m_pShadow_Thief->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBATIDLE) &&
		!m_pShadow_Thief->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT) &&
		!m_pShadow_Thief->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN))
	{
		return true;
	}

	return false;
}

//_bool CShadow_ThiefBT::IsReturn()
//{
//	if (!m_pShadow_Thief->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT))
//	{
//		_float4 vPos;
//		_float4 vOriginPos;
//
//		XMStoreFloat4(&vPos, m_tBTMonsterDesc.pOwnerTransform->Get_Position());
//		XMStoreFloat4(&vOriginPos, dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_OriginPos());
//
//		if (vPos.x >= vOriginPos.x - 0.1f && vPos.x <= vOriginPos.x + 0.1f &&
//			vPos.z >= vOriginPos.z - 0.1f && vPos.z <= vOriginPos.z + 0.1f)
//		{
//			return false;
//		}
//
//		return true;
//	}
//
//	return false;
//}


CShadow_ThiefBT* CShadow_ThiefBT::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CMonster* pOwner)
{
	CShadow_ThiefBT* pInstance = new CShadow_ThiefBT(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pOwner)))
	{
		MSG_BOX("Fail Create : CShadow_ThiefBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CShadow_ThiefBT::Clone(void* pArg)
{
	CShadow_ThiefBT* pInstance = new CShadow_ThiefBT(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CShadow_ThiefBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadow_ThiefBT::Free()
{
	__super::Free();

	Safe_Release(m_pRootNode);
}
