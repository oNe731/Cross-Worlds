#include "stdafx.h"
#include "Shadow_ThiefNode_Skill1.h"

#include "BehaviorTree.h"


#include "Model.h"

CShadow_ThiefNode_Skill1::CShadow_ThiefNode_Skill1()
{
}

HRESULT CShadow_ThiefNode_Skill1::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CShadow_ThiefNode_Skill1::Start()
{
	m_tBTMonsterDesc.pOwnerTransform->LookAt_ForLandObject(m_tBTMonsterDesc.pOwner->Get_TargetDesc().pTragetTransform->Get_Position());
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_ShadowThief.ao|ShadowThief_Skill01"));
	dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Set_Bools(CMonster::MONSTER_BOOLTYPE::MONBOOL_ATK, true);
}

CBTNode::NODE_STATE CShadow_ThiefNode_Skill1::Tick(const _float& fTimeDelta)
{
	// (���� �׼��� ���ϴ� �ִϸ��̼�, ������ ������ ��ٸ� �ð�, timeDelta)
	return __super::Atk_BehaviorTick(1.f, fTimeDelta);
}

CShadow_ThiefNode_Skill1* CShadow_ThiefNode_Skill1::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CShadow_ThiefNode_Skill1* pInstance = new CShadow_ThiefNode_Skill1();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CShadow_ThiefNode_Skill1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadow_ThiefNode_Skill1::Free()
{
	__super::Free();
}
