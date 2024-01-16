#pragma once
#include "Base.h"
#include "Engine_Defines.h"
#include "fmod.h"
#include "fmod.hpp"

BEGIN(Engine)

class ENGINE_DLL CSound_Manager : public CBase
{
	DECLARE_SINGLETON(CSound_Manager)	

private:
	explicit CSound_Manager();
	virtual  ~CSound_Manager();

public:
	HRESULT Reserve_Manager();

	void Tick(_float fTimeDelta);

public:
	void Play_Sound(wstring pSoundKey, CHANNELID eID, _float fVolume, _bool bStop = false, _float fCamDistance = 0.f);
	void Play_BGM(wstring pSoundKey, _float fVolume, _bool bStop = false, _float fFadeDuration = 0.5f);
	void Stop_Sound(CHANNELID eID, const _float fFadeOutDuration = 1.f);
	void Stop_All();
	void Set_ChannelVolume(CHANNELID eID, float fVolume);
	void Set_AllChannelVolume(_float fVolume) 
	{ 
		m_fAllChannelVolume = min(1.f, fVolume);
		m_fAllChannelVolume = max(0.f, m_fAllChannelVolume);
	}
	_float Get_AllChannelVolume() { return m_fAllChannelVolume; }

	_float Get_ChannelVolume(CHANNELID eID);
	FMOD_CHANNEL* Get_Channel(CHANNELID eID){ return m_pChannelArr[eID]; }
	const map<wstring, FMOD_SOUND*>& Get_MapSound() const { return m_mapSound; }

	const _int Get_SoundFileIndex(const wstring& pSoundKey);
	wstring Get_SoundFileKey(const _uint iIndex);
	wstring Get_SoundFileKey(string strKey);

private:
	void Search_Recursive(const std::string& currentPath);
	//void Load_SoundFile(const char* szSoundFilePath);
	void Play_Foot(CHANNELID eID, _float fVolume);

	void Tick_BgmBlending(_float fTimeDelta);
	

private:
	// ���� ���ҽ� ������ ���� ��ü 
	map<wstring, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	_float m_fSoundVolumeArr[MAXCHANNEL];
	_float m_fAllChannelVolume = 1.f;

	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;


	// BGM 
	_bool m_bInitBgm = false;
	_bool m_bStopBgm = false;
	_float m_fBgmFadeDuration = 0.5f; /* 0.5�ʵ��� ���� ����� ������ ���δ�. �ٽ� 0.5�� ���� ����� ������ ���δ�.*/
	_float m_fNextBgmVolume = 0.f;
	FMOD_SOUND* m_pNextBgmKey = nullptr;
	LERP_FLOAT_DESC m_tBgmVloumeDesc;


public:
	virtual void Free() override;

};

END