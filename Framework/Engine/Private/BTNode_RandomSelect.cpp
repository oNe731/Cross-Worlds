#include "BTNode_RandomSelect.h"

#include "GameInstance.h"

CBTNode_RandomSelect::CBTNode_RandomSelect()
{
}

HRESULT CBTNode_RandomSelect::Initialize_Prototype(CBehaviorTree* pBT)
{
	m_pBT = pBT;

	return S_OK;
}

void CBTNode_RandomSelect::Start()
{
}

CBTNode::NODE_STATE CBTNode_RandomSelect::Tick(const _float& fTimeDelta)
{
	// running �� �ƴ� ������ ������ �ؾ���.
	if (!m_bIsTemp)
	{

	}
	iIndex = GI->RandomInt(0, m_ltChildNode.size() - 1);

	auto iter = m_ltChildNode.begin();

	for (_int i = 0; i < iIndex; ++i)
		iter++;

	switch ((*iter)->Tick(fTimeDelta))
	{
	case NODE_STATE::NODE_RUNNING:

		break;
	case NODE_STATE::NODE_SUCCESS:
		return NODE_STATE::NODE_SUCCESS;
	}

	return NODE_STATE::NODE_FAIL;
}

CBTNode_RandomSelect* CBTNode_RandomSelect::Create(CBehaviorTree* pBT)
{
	CBTNode_RandomSelect* pInstance = new CBTNode_RandomSelect();

	if (FAILED(pInstance->Initialize_Prototype(pBT)))
	{
		MSG_BOX("Fail Create : CBTNode_RandomSelect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBTNode_RandomSelect::Free()
{
	__super::Free();
}
