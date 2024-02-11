#include "stdafx.h"
#include "QuestNode_Base.h"
#include "..\Public\QuestNode_Base.h"

#include "GameInstance.h"

#include "Camera_Group.h"

CQuestNode_Base::CQuestNode_Base()
{
}

HRESULT CQuestNode_Base::Initialize()
{
	return S_OK;
}

void CQuestNode_Base::Start()
{
}

CBTNode::NODE_STATE CQuestNode_Base::Tick(const _float& fTimeDelta)
{
	return NODE_STATE::NODE_RUNNING;
}

void CQuestNode_Base::LateTick(const _float& fTimeDelta)
{
}

Vec4 CQuestNode_Base::Set_DestSpot(CGameObject* pGameObject)
{
	CTransform* pTransform = pGameObject->Get_Component<CTransform>(TEXT("Com_Transform"));
	Vec4 vSpotPos = Vec4(pTransform->Get_Position()) + (Vec4(pTransform->Get_Look()).Normalized() * 2.5f);

	return vSpotPos;
}

void CQuestNode_Base::Delete_QuestDestSpot()
{
	Safe_Release(m_pQuestDestSpot);
}

const _bool CQuestNode_Base::Is_Finish_LevelEnterCameraAction()
{
	/* ���� ����� ����Ǵ� ī�޶� �׼��� �������� üũ�Ѵ�. (�÷��̾� ���·� �����ϸ� �ȵ�) */

	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return false;

	if (CAMERA_TYPE::ACTION == CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Key()
		&& CCamera_Action::CAMERA_ACTION_TYPE::DOOR == pActionCam->Get_Camera_ActionType())
		return false;

	if (CCamera_Manager::GetInstance()->Is_Blending_Camera())
		return false;

	/* �̰� ���� �÷��̾� ���·� üũ�ϴ� �ڵ� */
	//if (CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_CurrentState() == CCharacter::STATE::NEUTRAL_DOOR_ENTER)
	//	return NODE_STATE::NODE_RUNNING;

	return true;
}

const _bool CQuestNode_Base::Is_EndCameraBlender()
{
	// ���� ��� ī�޶� ������ ����, ���� ī�޶� �ȷο� ī�޶���
	if (!CCamera_Manager::GetInstance()->Is_Blending_Camera() &&
		CAMERA_TYPE::FOLLOW == CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Key())
	{
		return true;
	}

	return false;
}

void CQuestNode_Base::Free()
{
	__super::Free();
	Safe_Release(m_pQuestDestSpot);

	Safe_Delete_Array(m_szpOwner);
	Safe_Delete_Array(m_szpTalk);
}

