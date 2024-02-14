#include "stdafx.h"
#include "MainQuestNode_Invasion02.h"

#include "GameInstance.h"
#include "Utils.h"

#include "Game_Manager.h"
#include "UI_Manager.h"
#include "UI_PopupQuest.h"

#include "Player.h"
#include "Character.h"
#include "SkyDome.h"
#include "Quest_Manager.h"

#include "Camera_Group.h"
#include "Particle_Manager.h"

CMainQuestNode_Invasion02::CMainQuestNode_Invasion02()
{
}

HRESULT CMainQuestNode_Invasion02::Initialize()
{
	__super::Initialize();

	m_strQuestTag = TEXT("[����]");
	m_strQuestName = TEXT("�� ���� �Ҷ�");
	m_strQuestContent = TEXT("�� ������ ��������");

	m_strNextQuestTag = TEXT("[����]");
	m_strNextQuestName = TEXT("�ı��� ����Ÿ�����");
	m_strNextQuestContent = TEXT("�� �� ���ư��� ��Ȳ�� ���캸��");

	Json Load = GI->Json_Load(L"../Bin/DataFiles/Quest/MainQuest/05.MainQuest_Invasion/MainQuest_Invasion02.json");

	for (const auto& talkDesc : Load) {
		TALK_DELS sTalkDesc;
		sTalkDesc.strOwner = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Owner"]));
		sTalkDesc.strTalk = CUtils::PopEof_WString(CUtils::Utf8_To_Wstring(talkDesc["Talk"]));
		m_vecTalkDesc.push_back(sTalkDesc);
	}

	return S_OK;
}

void CMainQuestNode_Invasion02::Start()
{
	CUI_PopupQuest::QUEST_INFO QuestDesc = {};
	QuestDesc.strType = m_strQuestTag;
	QuestDesc.strTitle = m_strQuestName;
	QuestDesc.strContents = m_strQuestContent;
	CUI_Manager::GetInstance()->Set_QuestPopup(&QuestDesc);

	/* ���� ����Ʈ�� �����ִ� ��ü�� */
	m_pKuu = (CGameObject*)(CGame_Manager::GetInstance()->Get_Kuu());

}

CBTNode::NODE_STATE CMainQuestNode_Invasion02::Tick(const _float& fTimeDelta)
{
	if (m_bIsClear)
		return NODE_STATE::NODE_FAIL;

	if (GI->Get_CurrentLevel() == LEVELID::LEVEL_EVERMORE)
	{
		if (false == m_bBGMStart)
		{

			list<CGameObject*>& SkyDomes = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_SKYBOX);
			for (auto& pSkyDome : SkyDomes)
			{
				if (wstring::npos != pSkyDome->Get_PrototypeTag().find(L"Skydome"))
				{
					CSkyDome* pCastSky = dynamic_cast<CSkyDome*>(pSkyDome);
					if (nullptr != pCastSky)
					{
						CQuest_Manager::GetInstance()->Set_OriginSkyCenterColor(pCastSky->Get_CenterColor());
						CQuest_Manager::GetInstance()->Set_OriginSkyApexColor(pCastSky->Get_ApexColor());

						pCastSky->Set_CenterColor(Vec4(0.671f, 0.059f, 0.0f, 1.0f));
						pCastSky->Set_ApexColor(Vec4(0.329f, 0.173f, 0.157f, 1.0f));
					}
				}
			}
			CQuest_Manager::GetInstance()->Ready_InvasionLight(TEXT("Evermore Light"));

			CRenderer::FOG_DESC desc;
			::ZeroMemory(&desc, sizeof(desc));
			{
				desc.fFogDistanceValue = 30.0f;
				desc.fFogHeightValue = 50.0f;
				desc.fFogStartDepth = 100.0f;
				desc.fFogStartDistance = 0.04f;
				//desc.fFogHeightDensity = 0.110f;
				desc.fFogDistanceDensity = 0.110f;
			}
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_RendererCom()->Set_FogDesc(desc);
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_RendererCom()->Set_FogColor(Vec4(0.671f,0.059f,0.0f,1.0f));

			// Fog
			// Fog Color : r 0.671, g : 0.059, b : 0.0, a : 1.0f
			// FogDistvalue  : 30.0f
			// FogHeight 50.0
			// Depth 100, FogStartDist = 0.04 , Density = 0.110f
			// Renderer�� ������.

			// Light
			// Diffuse Ambient Upper 

			// Effect Change ------------------------
			list<CGameObject*>& Particles = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_EFFECT);
			for (auto& pParticle : Particles)
			{
				if (wstring::npos != pParticle->Get_PrototypeTag().find(L"Particle_Leaf"))
					pParticle->Set_Dead(true);
			}
			_matrix WorldMatrix = XMMatrixIdentity();
			WorldMatrix.r[3] = XMVectorSet(0.f, 30.f, 60.f, 1.f);
			GET_INSTANCE(CParticle_Manager)->AddLevel_Particle(LEVEL_EVERMORE, TEXT("Particle_FallFire_01"), WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
			GET_INSTANCE(CParticle_Manager)->AddLevel_Particle(LEVEL_EVERMORE, TEXT("Particle_FallFire_02"), WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
			GET_INSTANCE(CParticle_Manager)->AddLevel_Particle(LEVEL_EVERMORE, TEXT("Particle_FallFire_03"), WorldMatrix, _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
			// ------------------------ Effect Change

			GI->Stop_Sound(CHANNELID::SOUND_BGM_CURR, 0.f);
			GI->Play_BGM(L"BGM_Field_Hunting_CastleInside_Dark_1.ogg", 1.f, true);
			m_bBGMStart = true;
		}

		if (!m_bIsStart)
		{
			/* ���� ���� ī�޶� �׼��� �������̶�� ���� */
			if (!Is_Finish_LevelEnterCameraAction())
				return NODE_STATE::NODE_RUNNING;
			
			/* ��ȭ */
			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();

			/* ��ȭ ī�޶� ���� */
			CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
			if (nullptr != pActionCam)
				pActionCam->Start_Action_Talk(nullptr);

			m_bIsStart = true;
		}

		/* ���� ħ�� �ƽ� ������ 1�� ��ȭ ���̾�α� �ٷ� �߰� �ϱ� ���� */
		if (m_bStartCutScene && !m_bFinishCutScene)
		{
			if (CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Key() == CAMERA_TYPE::ACTION
				&& CUI_Manager::GetInstance()->Is_FadeFinished())
			{
				m_bFinishCutScene = true;

				Safe_Delete_Array(m_szpOwner);
				Safe_Delete_Array(m_szpTalk);

				m_iTalkIndex += 1;

				m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
				m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

				CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

				TalkEvent();
				return NODE_STATE::NODE_RUNNING;
			}
			else
			{
				return NODE_STATE::NODE_RUNNING;
			}
		}

		if (KEY_TAP(KEY::LBTN))
		{
			/* ���� ħ�� �ƽ� ���� (0��, 1�� ��ȭ ���� ����)*/
			if (0 == m_iTalkIndex)
			{
				if (!m_bStartCutScene)
				{
					m_bStartCutScene = true;
					CCamera_CutScene_Map* pCutSceneMap = dynamic_cast<CCamera_CutScene_Map*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE_MAP));
					if (nullptr != pCutSceneMap)
					{
						pCutSceneMap->Start_CutScene(LEVELID::LEVEL_EVERMORE, 1);
						pCutSceneMap->Reserve_NextCameraType(CAMERA_TYPE::ACTION);
						CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MAIN_DIALOG);
						return NODE_STATE::NODE_RUNNING;
					}
				}
			}

			Safe_Delete_Array(m_szpOwner);
			Safe_Delete_Array(m_szpTalk);

			m_iTalkIndex += 1;
			GI->Stop_Sound(CHANNELID::SOUND_UI);
			GI->Play_Sound(TEXT("UI_Fx_Comm_Btn_Dialogue_Page_1.mp3"), CHANNELID::SOUND_UI,
				GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

			if (m_iTalkIndex >= m_vecTalkDesc.size())
			{
				CUI_PopupQuest::QUEST_INFO QuestDesc = {};
				QuestDesc.strType = m_strNextQuestTag;
				QuestDesc.strTitle = m_strNextQuestName;
				QuestDesc.strContents = m_strNextQuestContent;
				CUI_Manager::GetInstance()->Update_QuestPopup(m_strQuestName, &QuestDesc);

				m_bIsClear = true;
				CUI_Manager::GetInstance()->OnOff_DialogWindow(false, CUI_Manager::MAIN_DIALOG);

				CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
				if (nullptr != pActionCam)
					pActionCam->Finish_Action_Talk();

				return NODE_STATE::NODE_FAIL;
			}

			m_szpOwner = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strOwner);
			m_szpTalk = CUtils::WStringToTChar(m_vecTalkDesc[m_iTalkIndex].strTalk);

			CUI_Manager::GetInstance()->Set_MainDialogue(m_szpOwner, m_szpTalk);

			TalkEvent();
		}
	}

	return NODE_STATE::NODE_RUNNING;
}

void CMainQuestNode_Invasion02::LateTick(const _float& fTimeDelta)
{
}

void CMainQuestNode_Invasion02::TalkEvent()
{
	CCamera_Action* pActionCam = dynamic_cast<CCamera_Action*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::ACTION));
	if (nullptr == pActionCam)
		return;

	switch (m_iTalkIndex)
	{
	case 0:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Invasion_02_00.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk01"));
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	case 1:
		CSound_Manager::GetInstance()->Play_Sound(TEXT("Invasion_02_01.ogg"), CHANNELID::SOUND_VOICE_CHARACTER, 1.f, true);
		m_pKuu->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CGameNpc::NPC_UNIQUENPC_TALK);
		m_pKuu->Get_Component<CModel>(TEXT("Com_Model"))->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_talk02"));
		pActionCam->Change_Action_Talk_Object(CCamera_Action::ACTION_TALK_DESC::KUU_AND_PLAYER);
		break;
	}
}

CMainQuestNode_Invasion02* CMainQuestNode_Invasion02::Create()
{
	CMainQuestNode_Invasion02* pInstance = new CMainQuestNode_Invasion02();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fail Create : CMainQuestNode_Invasion02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainQuestNode_Invasion02::Free()
{
	__super::Free();
}
