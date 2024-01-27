#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CCurlingGame_Manager : public CBase
{
	DECLARE_SINGLETON(CCurlingGame_Manager)

	typedef struct tagGuageDesc
	{
		_bool				bActive		= false;

		LERP_FLOAT_DESC		tLerpValue	= {};
		_bool				bIncrease	= true;

		const _float		fLerpTime	= 2.f;
		const LERP_MODE		eLerpMode	= LERP_MODE::EASE_IN;
		
		const _float		fMinValue	= 0.f;
		const _float		fMaxValue	= 1.f;

		void Start()
		{
			Reset();

			bActive = true;
			
			tLerpValue.Start(fMinValue, fMaxValue, fLerpTime, eLerpMode);
		}

		void Stop()
		{
			tLerpValue.bActive = false;
		}

		void Tick(const _float fTimeDelta)
		{
			if (tLerpValue.bActive)
			{
				tLerpValue.Update(fTimeDelta);
				if (!tLerpValue.bActive)
				{
					if (bIncrease)
						tLerpValue.Start(fMaxValue, fMinValue, fLerpTime, eLerpMode);
					else
						tLerpValue.Start(fMinValue, fMaxValue, fLerpTime, eLerpMode);

					bIncrease = !bIncrease;
				}
			}
		}

		void Reset()
		{
			tLerpValue.Clear();

			bIncrease = true;
		}
	}GUAGE_DESC;

	typedef struct tagStaiumDesc
	{
		_bool bActive = false;
		vector<CGameObject*>	pStadiumObjects;

	}STADIUM_DESC;

private:
	CCurlingGame_Manager();
	virtual ~CCurlingGame_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(const _float& fTimeDelta);
	void LateTick(const _float& fTimeDelta);

public:
	HRESULT Set_Game(const _bool& bStart);

public:
	/* Stadium */
	HRESULT Start_StadiumAction();
	HRESULT Finish_StaduimAction();

	/* UI */
	const _float& Get_GuageValue() const { return m_tGuageDesc.tLerpValue.fCurValue; }

private:
	void Tick_Guage(const _float& fTimeDelta);
	void Tick_StadiumAction(const _float& fTimeDelta);

private:
	HRESULT Ready_Objects();

private:
	void Test(const _float& fTimeDelta);

private:
	/* Default */
	ID3D11Device*			m_pDevice	= nullptr;
	ID3D11DeviceContext*	m_pContext	= nullptr;
	_bool					m_bReserved = false;

	/* Active */
	_bool					m_bPlaying	= false;

	/* Guage */
	GUAGE_DESC				m_tGuageDesc = {};

	/* Stadium */
	STADIUM_DESC			m_tStadiumDesc = {};

public:
	virtual void Free() override;
};

END
