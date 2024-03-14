#include "stdafx.h"
#include "Shadow_ThiefNode_Stun.h"

#include "GameInstance.h"
#include "BehaviorTree.h"

#include "Model.h"

CShadow_ThiefNode_Stun::CShadow_ThiefNode_Stun()
{
}

HRESULT CShadow_ThiefNode_Stun::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CShadow_ThiefNode_Stun::Start()
{
}

CBTNode::NODE_STATE CShadow_ThiefNode_Stun::Tick(const _float& fTimeDelta)
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

CShadow_ThiefNode_Stun* CShadow_ThiefNode_Stun::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CShadow_ThiefNode_Stun* pInstance = new CShadow_ThiefNode_Stun();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CShadow_ThiefNode_Stun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadow_ThiefNode_Stun::Free()
{
	__super::Free();
}
