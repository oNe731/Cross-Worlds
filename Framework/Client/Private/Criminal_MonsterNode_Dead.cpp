#include "stdafx.h"
#include "Criminal_MonsterNode_Dead.h"

#include "BehaviorTree.h"
#include "Model.h"
#include "RigidBody.h"

#include "Quest_Manager.h"

CCriminal_MonsterNode_Dead::CCriminal_MonsterNode_Dead()
{
}

HRESULT CCriminal_MonsterNode_Dead::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	m_fBlowDeadTime = 1.5f;

	return S_OK;
}

void CCriminal_MonsterNode_Dead::Start()
{
	if (dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD))
	{
		m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_BlackCircleAgent.ao|Down"));
		m_tBTMonsterDesc.pOwner->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Add_Velocity(
			-m_tBTMonsterDesc.pOwnerTransform->Get_Look()
			, dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Stat().fAirDeadVelocity, false);
	}
	else
	{
		m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_BlackCircleAgent.ao|Down"));
	}
}

CBTNode::NODE_STATE CCriminal_MonsterNode_Dead::Tick(const _float& fTimeDelta)
{
	if (m_tBTMonsterDesc.pOwnerModel->Is_Finish() && !m_tBTMonsterDesc.pOwnerModel->Is_Tween())
	{
		if (dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_BLOWDEAD))
		{
			if (m_tBTMonsterDesc.pOwner->Get_Component<CRigidBody>(TEXT("Com_RigidBody"))->Is_Ground())
			{
				m_fTime += fTimeDelta;
				if (m_fTime > m_fBlowDeadTime)
				{
					if (CQuest_Manager::GetInstance()->Get_CurQuestEvent() == CQuest_Manager::QUESTEVENT_MONSTER_KILL)
						CQuest_Manager::GetInstance()->Set_MonsterKillCount(1);

					// m_tBTMonsterDesc.pOwner->Reserve_Dead(true);
				}
			}
		}
		else
			if (CQuest_Manager::GetInstance()->Get_CurQuestEvent() == CQuest_Manager::QUESTEVENT_MONSTER_KILL)
				CQuest_Manager::GetInstance()->Set_MonsterKillCount(1);

			//m_tBTMonsterDesc.pOwner->Reserve_Dead(true);
	}

	return NODE_STATE::NODE_RUNNING;
}

CCriminal_MonsterNode_Dead* CCriminal_MonsterNode_Dead::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CCriminal_MonsterNode_Dead* pInstance = new CCriminal_MonsterNode_Dead();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CCriminal_MonsterNode_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCriminal_MonsterNode_Dead::Free()
{
	__super::Free();
}
