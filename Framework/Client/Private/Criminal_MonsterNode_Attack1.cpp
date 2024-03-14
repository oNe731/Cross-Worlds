#include "stdafx.h"
#include "Criminal_MonsterNode_Attack1.h"

#include "BehaviorTree.h"
#include "Model.h"

CCriminal_MonsterNode_Attack1::CCriminal_MonsterNode_Attack1()
{
}

HRESULT CCriminal_MonsterNode_Attack1::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CCriminal_MonsterNode_Attack1::Start()
{
	m_tBTMonsterDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTMonsterDesc.pOwner->Get_TargetDesc().pTragetTransform->Get_Position());
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_BlackCircleAgent.ao|Attack1"));
	dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
}

CBTNode::NODE_STATE CCriminal_MonsterNode_Attack1::Tick(const _float& fTimeDelta)
{
	// (���� �׼��� ���ϴ� �ִϸ��̼�, ������ ������ ��ٸ� �ð�, timeDelta)
	return __super::Atk_BehaviorTick(1.f, fTimeDelta);
}

CCriminal_MonsterNode_Attack1* CCriminal_MonsterNode_Attack1::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CCriminal_MonsterNode_Attack1* pInstance = new CCriminal_MonsterNode_Attack1();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CCriminal_MonsterNode_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCriminal_MonsterNode_Attack1::Free()
{
	__super::Free();
}
