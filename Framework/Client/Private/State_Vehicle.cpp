#include "stdafx.h"
#include "GameInstance.h"
#include "Character.h"
#include "State_Vehicle.h"
#include "UI_Manager.h"
#include "UI_Dialog_Window.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Vehicle.h"

#include "Camera_Manager.h"
#include "Camera_Follow.h"

CState_Vehicle::CState_Vehicle(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{

}



HRESULT CState_Vehicle::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);
	m_pVehicle = dynamic_cast<CVehicle*>(m_pStateMachineCom->Get_Owner());
	if (nullptr == m_pVehicle)
		return E_FAIL;

	return S_OK;
}

void CState_Vehicle::Free()
{
	__super::Free();
}


void CState_Vehicle::Reset_Camera_WideToDeafult()
{
	/* �ȷο� ī�޶� ���̵�� ���¿���, ���� �÷��̾ ��ų Ȥ�� ������ ����Ѵٸ� ����Ʈ��� ������ ��ȯ�Ѵ�. */

	CCamera* pCurCam = CCamera_Manager::GetInstance()->Get_CurCamera();
	if (nullptr != pCurCam && CAMERA_TYPE::FOLLOW)
	{
		CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(pCurCam);
		if (nullptr != pFollowCam)
			pFollowCam->Reset_WideView_To_DefaultView(false, 0.5f);
	}
}

