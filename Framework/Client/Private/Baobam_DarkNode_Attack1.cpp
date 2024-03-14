#include "stdafx.h"
#include "Baobam_DarkNode_Attack1.h"

#include "BehaviorTree.h"
#include "Model.h"

CBaobam_DarkNode_Attack1::CBaobam_DarkNode_Attack1()
{
}

HRESULT CBaobam_DarkNode_Attack1::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CBaobam_DarkNode_Attack1::Start()
{
	m_tBTMonsterDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTMonsterDesc.pOwner->Get_TargetDesc().pTragetTransform->Get_Position());
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_Baobam_Darkness.ao|BaoBam_Attack01"));
	dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
}

CBTNode::NODE_STATE CBaobam_DarkNode_Attack1::Tick(const _float& fTimeDelta)
{
	return __super::Atk_BehaviorTick(TEXT("SKM_Baobam_Darkness.ao|BaoBam_Attack01"), 1.f, fTimeDelta);
}

CBaobam_DarkNode_Attack1* CBaobam_DarkNode_Attack1::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CBaobam_DarkNode_Attack1* pInstance = new CBaobam_DarkNode_Attack1();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CBaobam_DarkNode_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaobam_DarkNode_Attack1::Free()
{
	__super::Free();
}
