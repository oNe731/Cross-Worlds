#include "stdafx.h"
#include "Baobam_DarkNode_Stun.h"

#include "BehaviorTree.h"
#include "BlackBoard.h"

#include "Model.h"

CBaobam_DarkNode_Stun::CBaobam_DarkNode_Stun()
{
}

HRESULT CBaobam_DarkNode_Stun::Initialize_Prototype(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CBaobam_DarkNode_Stun::Start()
{
}

CBTNode::NODE_STATE CBaobam_DarkNode_Stun::Tick(const _float& fTimeDelta)
{
	if (!dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN))
		return NODE_STATE::NODE_FAIL;

	// ���� �� �ǰ�
	if (dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HIT))
	{
		dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HIT, false);
	}

	m_fTime += fTimeDelta;

	if (m_fTime >= dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_StunTime())
	{
		m_fTime = dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_StunTime() - m_fTime;

		dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN, false);
		dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HIT, false);
		dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_HITANIM, false);

		if (dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND))
			dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
		else
			dynamic_cast<CMonster*>(m_tBTNodeDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, false);

		return NODE_STATE::NODE_SUCCESS;
	}

	return NODE_STATE::NODE_RUNNING;
}

CBaobam_DarkNode_Stun* CBaobam_DarkNode_Stun::Create(BTNODE_DESC* pDesc, CBehaviorTree* pBT)
{
	CBaobam_DarkNode_Stun* pInstance = new CBaobam_DarkNode_Stun();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CBaobam_DarkNode_Stun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaobam_DarkNode_Stun::Free()
{
	__super::Free();
}
