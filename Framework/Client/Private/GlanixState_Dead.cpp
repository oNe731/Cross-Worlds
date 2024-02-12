#include "stdafx.h"
#include "GlanixState_Dead.h"

#include "Glanix.h"

#include "Game_Manager.h"
#include "Player.h"

#include "Camera_Group.h"

#include "Quest_Manager.h"
#include "UI_Manager.h"
#include "UI_Fade.h"

#include "GameInstance.h"

CGlanixState_Dead::CGlanixState_Dead(CStateMachine* pStateMachine)
	: CGlanixState_Base(pStateMachine)
{
}

HRESULT CGlanixState_Dead::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CGlanixState_Dead::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Glanix.ao|Glanix_Death"));

	/* �׶�н� ������ �� �� �ʱ�ȭ */
	{
		m_pGlanix->Get_Component_Transform()->Set_Position(m_pGlanix->Get_OriginPos());
		m_pGlanix->Get_Component_Transform()->Set_LookAtByDir(m_pGlanix->Get_OriginLook());
	}

	/* �׶�н� �״� �ƽ� ���� */
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr != pActionCam)
	{
		pActionCam->Start_Action_Glanix_Dead(m_pGlanix);
		CCamera_Manager::GetInstance()->Set_CurCamera(pActionCam->Get_Key());
	}

	/* �÷��̾�� �ٸ� ���������� �Űܵд�. */
	{
		CTransform* pPlayerTransform = CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component_Transform();
		if (nullptr != pPlayerTransform)
		{
			pPlayerTransform->Set_Position({ -44.f, 2.f, 330.f, 1.f }); /* ��Ż ��ġ*/
			pPlayerTransform->LookAt_ForLandObject(m_pGlanix->Get_OriginPos()); /* �׶�н� ���� ��ġ */

			/* ��Ż ��ġ���� �׶�н� ���� ��ġ �������� n��ŭ ������ ��ġ */
			Vec4 vFinalPos = Vec4(pPlayerTransform->Get_Position()) + Vec4(pPlayerTransform->Get_Look()).ZeroY().Normalized() * 20.f; 

			pPlayerTransform->Set_Position(vFinalPos.OneW());
		}
	}
}

void CGlanixState_Dead::Tick_State(_float fTimeDelta)
{
	if (!m_bStartFadeOut && 0.85 <= m_pModelCom->Get_Progress() && !m_pModelCom->Is_Tween())
	{
		m_bStartFadeOut = true;
		CUI_Manager::GetInstance()->Get_Fade()->Set_Fade(true, 1.5f);
	}

	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
	{
		m_pModelCom->Set_Stop_Animation(true);

		if (CUI_Manager::GetInstance()->Is_FadeFinished())
		{
			if(CQuest_Manager::GetInstance()->Get_CurQuestEvent() == CQuest_Manager::QUESTEVENT_BOSS_KILL)
				CQuest_Manager::GetInstance()->Set_IsBossKill(true);

			m_pGlanix->Reserve_Dead(true);

			/* �״� �ƽ� ó���� ������ ���� ���� */
			m_pGlanix->Set_DissolveDuration(0.1f);

			GI->Play_BGM(TEXT("BGM_Field_Village_Winter_Po_1.mp3"), GI->Get_ChannelVolume(CHANNELID::SOUND_BGM_CURR), false, 0.5f);
		}
	}
}

void CGlanixState_Dead::Exit_State()
{

}

CGlanixState_Dead* CGlanixState_Dead::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CGlanixState_Dead* pInstance = new CGlanixState_Dead(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CGlanixState_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanixState_Dead::Free()
{
	__super::Free();
}
