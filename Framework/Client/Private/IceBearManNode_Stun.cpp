#include "stdafx.h"
#include "IceBearManNode_Stun.h"

#include "BehaviorTree.h"

#include "GameInstance.h"
#include "Model.h"

CIceBearManNode_Stun::CIceBearManNode_Stun()
{
}

HRESULT CIceBearManNode_Stun::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CIceBearManNode_Stun::Start()
{
}

CBTNode::NODE_STATE CIceBearManNode_Stun::Tick(const _float& fTimeDelta)
{
	if (!dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN))
		return NODE_STATE::NODE_FAIL;

	m_fTime += fTimeDelta;

	if (m_fTime >= dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_StunTime())
	{
		m_fTime = dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_StunTime() - m_fTime;

		dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_STUN, false);
		dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_WEAK, false);
		dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ISHIT, false);

		if (dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATKAROUND))
			dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
		else
			dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, false);

		return NODE_STATE::NODE_SUCCESS;
	}

	return NODE_STATE::NODE_RUNNING;
}

CIceBearManNode_Stun* CIceBearManNode_Stun::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CIceBearManNode_Stun* pInstance = new CIceBearManNode_Stun();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CIceBearManNode_Stun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIceBearManNode_Stun::Free()
{
	__super::Free();
}
