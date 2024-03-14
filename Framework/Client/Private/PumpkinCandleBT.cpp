#include "stdafx.h"
#include "PumpkinCandleBT.h"

#include "GameInstance.h"

#include "PumpkinCandle.h"

#include "BTNode_Condition.h"
#include "BTNode_Leaf.h"
#include "BTNode_Select.h"
#include "BTNode_RandomSelect.h"
#include "BTNode_Sequence.h"
#include "BTNode_Loop.h"

#include "PumpkinCandleNode_Dead.h"

#include "PumpkinCandleNode_Blow.h"
#include "PumpkinCandleNode_Air.h"
#include "PumpkinCandleNode_Stun.h"
#include "PumpkinCandleNode_Hit.h"

#include "PumpkinCandleNode_Attack1.h"
#include "PumpkinCandleNode_Attack2.h"

#include "PumpkinCandleNode_Chase.h"

#include "PumpkinCandleNode_Roaming.h"
#include "PumpkinCandleNode_Idle.h"

#include "Game_Manager.h"
#include "Player.h"


CPumpkinCandleBT::CPumpkinCandleBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonsterBT(pDevice, pContext)
{
}

CPumpkinCandleBT::CPumpkinCandleBT(const CPumpkinCandleBT& rhs)
	: CMonsterBT(rhs)
{
}

HRESULT CPumpkinCandleBT::Initialize_Prototype(CMonster* pOwner)
{
	__super::Initialize_Prototype(pOwner);

	m_pPumpkinCandle = dynamic_cast<CPumpkinCandle*>(m_tBTMonsterDesc.pOwner);
	m_pRootNode = CBTNode_Select::Create(this);

	/* ���� Sequence ���� */
	CBTNode_Sequence* pSeq_Dead = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Hit = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Combat = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Chase = CBTNode_Sequence::Create(this);
	//CBTNode_Sequence* pSeq_Return = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Idle = CBTNode_Sequence::Create(this);

	/* Dead ���� */
	CPumpkinCandleNode_Dead* pDeadNode = CPumpkinCandleNode_Dead::Create(&m_tBTMonsterDesc, this);

	/* Hit ���� */
	CBTNode_Select* pSel_Hit = CBTNode_Select::Create(this);
	CPumpkinCandleNode_Blow* pBlowNode = CPumpkinCandleNode_Blow::Create(&m_tBTMonsterDesc, this);
	CPumpkinCandleNode_Air* pAirNode = CPumpkinCandleNode_Air::Create(&m_tBTMonsterDesc, this);
	CPumpkinCandleNode_Stun* pStunNode = CPumpkinCandleNode_Stun::Create(&m_tBTMonsterDesc, this);
	CPumpkinCandleNode_Hit* pHitNode = CPumpkinCandleNode_Hit::Create(&m_tBTMonsterDesc, this);

	/* Combat ���� */
	CBTNode_Sequence* pSeq_Pattern = CBTNode_Sequence::Create(this);
	CPumpkinCandleNode_Attack1* pAtk1Node = CPumpkinCandleNode_Attack1::Create(&m_tBTMonsterDesc, this);
	CPumpkinCandleNode_Attack2* pAtk2Node = CPumpkinCandleNode_Attack2::Create(&m_tBTMonsterDesc, this);

	/* Chase ���� */
	CPumpkinCandleNode_Chase* pChaseNode = CPumpkinCandleNode_Chase::Create(&m_tBTMonsterDesc, this);

	/* Return ���� */
	//CBaobam_WaterNode_Return* pReturnNode = CBaobam_WaterNode_Return::Create(&m_tBTMonsterDesc, this);

	/* Idel ���� */
	vector<wstring> vecAnimationName;
	vecAnimationName.push_back(TEXT("SKM_PumpkinCandle.ao|PumpkinCandle_BattleStand"));
	vecAnimationName.push_back(TEXT("SKM_PumpkinCandle.ao|PumpkinCandle_BattleStand"));
	CPumpkinCandleNode_Idle* pIdleNode = CPumpkinCandleNode_Idle::Create(&m_tBTMonsterDesc, this, vecAnimationName);
	CPumpkinCandleNode_Roaming* pRoamingNode = CPumpkinCandleNode_Roaming::Create(&m_tBTMonsterDesc, this, dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_RoamingArea());

	/* Condition ����*/
	/* function<_bool()>�� �޴� CBTNode_Condition::Create �Լ������� ��� �Լ��� ����ϰ� �ֱ� ������ �߰����� ó���� �ʿ� */
	CBTNode_Condition* pCon_IsDead = CBTNode_Condition::Create(bind(&CPumpkinCandleBT::IsZeroHp, this), pDeadNode, pHitNode);
	CBTNode_Condition* pCon_IsHit = CBTNode_Condition::Create(bind(&CPumpkinCandleBT::IsHit, this), pHitNode, pChaseNode);
	CBTNode_Condition* pCon_IsCombat = CBTNode_Condition::Create(bind(&CPumpkinCandleBT::IsAtk, this), nullptr, pChaseNode);
	CBTNode_Condition* pCon_IsChase = CBTNode_Condition::Create(bind(&CPumpkinCandleBT::IsChase, this), pChaseNode, pRoamingNode);
	//CBTNode_Condition* pCon_IsReturn = CBTNode_Condition::Create(bind(&CPumpkinCandleBT::IsReturn, this), pReturnNode, pIdleNode);


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

HRESULT CPumpkinCandleBT::Initialize(void* pArg)
{
	return S_OK;
}

void CPumpkinCandleBT::Tick(const _float& fTimeDelta)
{
	if (m_tBTMonsterDesc.pOwner != nullptr)
		m_pRootNode->Tick(fTimeDelta);
}

void CPumpkinCandleBT::LateTick(const _float& fTimeDelta)
{
}

void CPumpkinCandleBT::Init_NodeStart()
{
	m_pRootNode->Init_Start();
}

_bool CPumpkinCandleBT::IsZeroHp()
{
	if (m_pPumpkinCandle->Get_Stat().fHp <= 0)
		return true;

	return false;
}

_bool CPumpkinCandleBT::IsHit()
{
	if (m_pPumpkinCandle->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT))
		return true;

	return false;
}

_bool CPumpkinCandleBT::IsAtk()
{
	if (m_pPumpkinCandle->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT))
	{
		if (m_pPumpkinCandle->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND) ||
			m_pPumpkinCandle->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK) ||
			m_pPumpkinCandle->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBATIDLE))
		{
			return true;
		}
	}

	return false;
}

_bool CPumpkinCandleBT::IsChase()
{
	if (m_pPumpkinCandle->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT) &&
		!m_pPumpkinCandle->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK) &&
		!m_pPumpkinCandle->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBATIDLE) &&
		!m_pPumpkinCandle->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT) &&
		!m_pPumpkinCandle->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN))
	{
		return true;
	}

	return false;
}

//_bool CPumpkinCandleBT::IsReturn()
//{
//	if (!m_pBaobam_Water->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_COMBAT))
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


CPumpkinCandleBT* CPumpkinCandleBT::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CMonster* pOwner)
{
	CPumpkinCandleBT* pInstance = new CPumpkinCandleBT(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pOwner)))
	{
		MSG_BOX("Fail Create : CPumpkinCandleBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CPumpkinCandleBT::Clone(void* pArg)
{
	CPumpkinCandleBT* pInstance = new CPumpkinCandleBT(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CPumpkinCandleBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPumpkinCandleBT::Free()
{
	__super::Free();

	Safe_Release(m_pRootNode);
}
