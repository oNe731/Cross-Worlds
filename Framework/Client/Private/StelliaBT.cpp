#include "stdafx.h"
#include "StelliaBT.h"

#include "GameInstance.h"

#include "Stellia.h"

#include "BTNode_Condition.h"
#include "BTNode_Leaf.h"
#include "BTNode_Select.h"
#include "BTNode_Sequence.h"

#include "StelliaNode_Dead.h"

#include "StelliaNode_Chase.h"

#include "StelliaNode_Scratch.h"
#include "StelliaNode_DoubleScratch.h"

CStelliaBT::CStelliaBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBehaviorTree(pDevice, pContext)
{
}

CStelliaBT::CStelliaBT(const CStelliaBT& rhs)
	: CBehaviorTree(rhs)
{
}

HRESULT CStelliaBT::Initialize_Prototype(CGameObject* pObject)
{
	m_BlackBoard.emplace(TEXT("Test1"), new tagBlackBoardData<CGameObject*>((CGameObject*)this));
	m_BlackBoard.emplace(TEXT("Test2"), new tagBlackBoardData<_float>(4.f));

	m_pStellia = dynamic_cast<CStellia*>(pObject);
	m_pRootNode = CBTNode_Select::Create();

	/* ���� Sequence ���� */
	CBTNode_Sequence* pSeq_Dead = CBTNode_Sequence::Create();
	CBTNode_Sequence* pSeq_Chase = CBTNode_Sequence::Create();
	CBTNode_Sequence* pSeq_Combat = CBTNode_Sequence::Create();
	
	/* Dead ���� */
	/* function<_bool()>�� �޴� CBTNode_Condition::Create �Լ������� ��� �Լ��� ����ϰ� �ֱ� ������ �߰����� ó���� �ʿ� */
	CBTNode_Condition* pCon_IsDead = CBTNode_Condition::Create(bind(&CStelliaBT::IsZeroHp, this));
	CStelliaNode_Dead* pDeadNode = CStelliaNode_Dead::Create(this);
	
	/* Chase ���� */
	CBTNode_Condition* pCon_IsChase = CBTNode_Condition::Create(bind(&CStelliaBT::IsChase, this));
	CStelliaNode_Chase* pChaseNode = CStelliaNode_Chase::Create(this);

	/* ���� Ȯ�� */
	CBTNode_Condition* pCon_IsBerserk = CBTNode_Condition::Create(bind(&CStelliaBT::IsBerserk, this));

	/* Combat(Normal) ���� */
	//CBTNode_Sequence* pSeq_NmPatternRandom = CBTNode_Sequence::Create();
	CStelliaNode_Scratch* pScratchNode = CStelliaNode_Scratch::Create(this);
	CStelliaNode_DoubleScratch* pDoubleScratchNode = CStelliaNode_DoubleScratch::Create(this);

	/* Combat(Berserk) ���� */

	m_pRootNode->Add_ChildNode(pSeq_Dead); // root.push(dead Sequence)
	pSeq_Dead->Add_ChildNode(pCon_IsDead); // dead Sequence.push(dead Condition)
	pSeq_Dead->Add_ChildNode(pDeadNode); // dead Sequence.push(dead Lerp)

	m_pRootNode->Add_ChildNode(pSeq_Chase); // root.push(chase Sequence)
	pSeq_Chase->Add_ChildNode(pCon_IsChase); // cahse Sequence.push(chase Condition)
	pSeq_Chase->Add_ChildNode(pChaseNode); // chase Sequence.push(chase Lerp)

	m_pRootNode->Add_ChildNode(pSeq_Combat); // root.push(combat Sequence)
	pSeq_Combat->Add_ChildNode(pCon_IsBerserk); // combat Sequence.push(combat Condition)
	pSeq_Combat->Add_ChildNode(pScratchNode); // scratch Sequence.push(combat Lerp)
	pSeq_Combat->Add_ChildNode(pDoubleScratchNode); // doubleScratch Sequence.push(combat Lerp)

	return S_OK;
}

HRESULT CStelliaBT::Initialize(void* pArg)
{
	return S_OK;
}

void CStelliaBT::Tick(const _float& fTimeDelta)
{
	m_pRootNode->Tick(fTimeDelta);
}

void CStelliaBT::LateTick(const _float& fTimeDelta)
{
	IsZeroHp();
	IsChase();
	IsBerserk();
}

_bool CStelliaBT::IsZeroHp()
{
	if (m_pStellia->Get_Stat().fHp <= 0)
		return true;

	return false;
}

_bool CStelliaBT::IsChase()
{
	// ���� �Ÿ���� �ʿ�

	//if (m_pStellia->Get_Stat().fHp <= 0)
	//	return true;
	//
	//return false;
	return false;
}

_bool CStelliaBT::IsBerserk()
{
	// ���� ���� �˻� �ʿ�
	
	//if (m_pStellia->Get_Stat().fHp <= 0)
	//	return true;
	//
	//return false;
	return true;
}

CStelliaBT* CStelliaBT::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pObject)
{
	CStelliaBT* pInstance = new CStelliaBT(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pObject)))
	{
		MSG_BOX("Fail Create : CStelliaBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CStelliaBT::Clone(void* pArg)
{
	CStelliaBT* pInstance = new CStelliaBT(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CStelliaBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaBT::Free()
{
	__super::Free();

	Safe_Release(m_pRootNode);
}
