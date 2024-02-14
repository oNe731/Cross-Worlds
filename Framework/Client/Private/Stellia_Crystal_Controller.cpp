#include "stdafx.h"
#include "Stellia_Crystal_Controller.h"

#include "GameInstance.h"

#include "Stellia.h"
#include "Stellia_Crystal.h"
#include "Stellia_Crystal_Destructible.h"
#include "Effect.h"

#include "UI_Stellia_Timer.h"

CStellia_Crystal_Controller::CStellia_Crystal_Controller()
{
	m_fRespawnTime = 3.f;
	m_iCrystalAmount = 6;
	m_fSlowTime = 0.1f;
	m_fLimitTime = 30.f;

	m_vecCrystalType.push_back(CRYSTAL_AURA);
	m_vecCrystalType.push_back(CRYSTAL_SKY);
	m_vecCrystalType.push_back(CRYSTAL_GOLD);
	m_vecCrystalType.push_back(CRYSTAL_AURA);
	m_vecCrystalType.push_back(CRYSTAL_SKY);
	m_vecCrystalType.push_back(CRYSTAL_GOLD);
}

void CStellia_Crystal_Controller::Tick(const _float fTimeDelta)
{
	if (nullptr == m_pStellia)
		return;

	if(m_pStellia->Get_Bools(CStellia::BOSS_BOOLTYPE::BOSSBOOL_RAGE2))
	{
		// ���� ���� ���϶� ���� �ð��� �����ٸ�.(Fail)
		if (m_bIsProgress)
		{
			m_fAccLimitTime += fTimeDelta;
			if (m_fAccLimitTime >= m_fLimitTime)
			{
				m_pStellia->Get_StelliaTimer()->Set_Active(false);

				m_pStellia->Set_CrystalFailCount(1);
				m_pStellia->Clear_CrystalBingoCount();
				Clear_Crystals();
				Clear_Progress();
			}
		}

		if (m_pStellia->Get_CrystalBingoCount() >= 2)
		{
			m_pStellia->Get_StelliaTimer()->Set_Active(false);

			// Ÿ�� ���� ����.
			//if (!m_bIsTimeSlep)
			//{
			//	GI->Set_TimeScale(TIMER_TYPE::GAME_PLAY, 0.05f);
			//	m_bIsTimeSlep = true;
			//	m_bIsSlow = true;
			//}

			Clear_Crystals();
			Clear_Progress();
			m_pStellia->Clear_CrystalBingoCount();
			m_pStellia->Set_CrystalSuccessCount(1);
		}

		if (m_bIsRespawn)
		{
			if (m_pStellia->Get_CrystalSuccessCount() < 3 && m_pStellia->Get_CrystalFailCount() < 3)
			{
				m_fAccRespawnTime += fTimeDelta;
				if (m_fAccRespawnTime >= m_fRespawnTime)
				{
					m_fAccRespawnTime = m_fRespawnTime - m_fAccRespawnTime;
					m_bIsRespawn = false;
					Create_Crystals(m_pStellia);
				}
			}
		}

		if (m_pStellia->Get_CrystalSuccessCount() == 3)
		{
			m_pStellia->Get_StelliaTimer()->Set_Active(false);

			if (m_pOriginalCrystal != nullptr)
				Safe_Release(m_pOriginalCrystal);

			for (auto& pCrystal : m_pCrystals)
			{
				pCrystal->Set_Dead(true);
			}

			m_pCrystals.clear();
			m_pCrystals.shrink_to_fit();

			_float fStunTime = 15.f;

			m_pStellia->Set_Bools(CStellia::BOSS_BOOLTYPE::BOSSBOOL_RAGE2, false);
			m_pStellia->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CStellia::STELLIA_COUNTERSTART, &fStunTime);
		}
		else if (m_pStellia->Get_CrystalFailCount() == 3)
		{
			m_pStellia->Get_StelliaTimer()->Set_Active(false);

			if (m_pOriginalCrystal != nullptr)
				Safe_Release(m_pOriginalCrystal);

			for (auto& pCrystal : m_pCrystals)
			{
				pCrystal->Set_Dead(true);
			}

			m_pCrystals.clear();
			m_pCrystals.shrink_to_fit();

			m_pStellia->Set_Bools(CStellia::BOSS_BOOLTYPE::BOSSBOOL_RAGE2, false);
			m_pStellia->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Change_State(CStellia::STELLIA_RAGE2FINISH);
		}

		// Ÿ�� ����
		//if (m_bIsTimeSlep)
		//{
		//	if (m_bIsSlow)
		//	{
		//		m_fSleepTime += fTimeDelta;
		//
		//		if (m_fSleepTime >= m_fSlowTime)
		//		{
		//			m_fSleepTime = 0.f;
		//			m_bIsTimeSlep = false;
		//			m_bIsSlow = false;
		//			GI->Set_TimeScale(TIMER_TYPE::GAME_PLAY, 1.f);
		//		}
		//	}
		//}
	}
}

HRESULT CStellia_Crystal_Controller::Create_Crystals(CStellia* pStellia)
{
	CSound_Manager::GetInstance()->Play_Sound(TEXT("CreateCrystal.ogg"), CHANNELID::SOUND_VOICE_WITCH_QUEST, 1.f, false);

	m_bIsProgress = true;

	if (pStellia == nullptr)
		return E_FAIL;

	m_iOriginalType = GI->RandomInt(CRYSTAL_AURA, CRYSTAL_GOLD);
	m_pStellia = pStellia;
	m_vOriginPos = m_pStellia->Get_OriginPos();

	m_pStellia->Get_StelliaTimer()->Set_Timer(30.f);
	m_pStellia->Get_StelliaTimer()->Set_Active(true);

	/* ���� ũ����Ż ���� */
	CGameObject* pGameObject = nullptr;
	GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_PROP), TEXT("Prorotype_GameObject_Stellia_Crystal"), &m_vOriginPos, &pGameObject);
	if (nullptr != pGameObject)
	{
		CStellia_Crystal* pOriginalCrystal = dynamic_cast<CStellia_Crystal*>(pGameObject);
		if (nullptr != pOriginalCrystal)
		{
			pOriginalCrystal->Set_CrystalType(m_iOriginalType);

			// �� ����Ʈ ���� ����
			/*
			CGameObject* pGameObject = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_EFFECT, L"Effect_Stellia_Shield");
			if (nullptr != pGameObject)
			{
				CEffect* pEffect = dynamic_cast<CEffect*>(pGameObject);
				if (nullptr != pEffect)
				{
					Vec3 vColor = {};
					switch (m_iOriginalType)
					{
					case 0: // CRYSTAL_AURA
						vColor = Vec3(0.212f, 0.620f, 1.f);
						break;
					case 1: // CRYSTAL_SKY
						vColor = Vec3(0.715f, 0.995f, 1.f);
						break;
					case 2: // CRYSTAL_GOLD
						vColor = Vec3(1.f, 0.96f, 0.466f);
						break;
					}

					pEffect->Set_Color(vColor);
				}
			}
			*/
		}
	}

	/* �ʵ� ũ����Ż ���� */
	if (nullptr == m_pStellia)
		return E_FAIL;

	// ����
	for (_int i = 0; i < 100; ++i)
	{
		_int iIndex1 = GI->RandomInt(0, 5);
		_int iIndex2 = GI->RandomInt(0, 5);

		if (iIndex1 == iIndex2)
		{
			if (i != 0)
				i -= 1;

			continue;
		}

		CRYSTAL_TYPE eTemp = m_vecCrystalType[iIndex1];
		m_vecCrystalType[iIndex1] = m_vecCrystalType[iIndex2];
		m_vecCrystalType[iIndex2] = eTemp;
	}

	// ����
	for (size_t i = 0; i < m_iCrystalAmount; i++)
	{
		const float fAngle = 360.f / (_float)m_iCrystalAmount * i;

		_float fRadius = GI->RandomFloat(15.f, 30.f);

		Vec4 vPos = {};
		{
			vPos.x = m_vOriginPos.x + fRadius * cos(XMConvertToRadians(fAngle));
			if (i % 2 != 0)
			{
				vPos.y = m_vOriginPos.y + 3.f;
			}
			else
			{
				vPos.y = m_vOriginPos.y + 0.5f;
			}
			vPos.z = m_vOriginPos.z + fRadius * sin(XMConvertToRadians(fAngle));
			vPos.w = 1.f;
		}

		pGameObject = nullptr;
		GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_MONSTER), TEXT("Prorotype_GameObject_Stellia_Crystal_Destructible"), &vPos, &pGameObject);

		if (nullptr != pGameObject)
		{
			CStellia_Crystal_Destructible* pCrystal = dynamic_cast<CStellia_Crystal_Destructible*>(pGameObject);
			if (nullptr != pCrystal)
			{
				/* Set Data */
				CStellia_Crystal_Destructible::STELLIA_CRYSTAL_DESC tCrystalDesc;
				tCrystalDesc.pStellia = m_pStellia;
				tCrystalDesc.iBingoType = m_iOriginalType;
				tCrystalDesc.iSelfType = m_vecCrystalType[i];

				pCrystal->Set_CrystalData(&tCrystalDesc);

				m_pCrystals.push_back(pCrystal);
			}
		}
	}

	return S_OK;
}

HRESULT CStellia_Crystal_Controller::Clear_Crystals()
{
	CSound_Manager::GetInstance()->Play_Sound(TEXT("CreateCrystal.ogg"), CHANNELID::SOUND_VOICE_WITCH_QUEST, 1.f, false);

	for (auto& pCrystal : m_pCrystals)
	{
		pCrystal->Set_Dead(true);
	}

	m_pCrystals.clear();
	m_pCrystals.shrink_to_fit();

	return S_OK;
}

void CStellia_Crystal_Controller::Clear_Progress()
{
	m_bIsRespawn = true;
	m_bIsProgress = false;
	m_fAccLimitTime = 0.f;
}

void CStellia_Crystal_Controller::Set_CrystalTurnData()
{
	for (_int i = 0; i < m_pCrystals.size(); ++i)
	{
		if (m_pCrystals[i] != nullptr)
			m_pCrystals[i]->Set_CrystalTurnData();
	}
}
