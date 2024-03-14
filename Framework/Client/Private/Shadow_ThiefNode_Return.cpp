#include "stdafx.h"
#include "Shadow_ThiefNode_Return.h"

#include "BehaviorTree.h"


#include "Model.h"

CShadow_ThiefNode_Return::CShadow_ThiefNode_Return()
{
}

HRESULT CShadow_ThiefNode_Return::Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	__super::Initialize_Prototype(pDesc, pBT);

	return S_OK;
}

void CShadow_ThiefNode_Return::Start()
{
	m_tBTMonsterDesc.pOwnerModel->Set_Animation(TEXT("SKM_ShadowThief.ao|ShadowThief_Walk"));
}

CBTNode::NODE_STATE CShadow_ThiefNode_Return::Tick(const _float& fTimeDelta)
{
	m_tBTMonsterDesc.pOwnerTransform->LookAt_ForLandObject(dynamic_cast<CMonster*>(m_tBTMonsterDesc.pOwner)->Get_OriginPos());
	m_tBTMonsterDesc.pOwnerTransform->Move(m_tBTMonsterDesc.pOwnerTransform->Get_Look(), 3.f, fTimeDelta);

	return NODE_STATE::NODE_RUNNING;
}

CShadow_ThiefNode_Return* CShadow_ThiefNode_Return::Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT)
{
	CShadow_ThiefNode_Return* pInstance = new CShadow_ThiefNode_Return();

	if (FAILED(pInstance->Initialize_Prototype(pDesc, pBT)))
	{
		MSG_BOX("Fail Create : CShadow_ThiefNode_Return");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadow_ThiefNode_Return::Free()
{
	__super::Free();
}
