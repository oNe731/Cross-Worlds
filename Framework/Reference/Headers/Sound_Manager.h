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

public:
	void Play_Sound(wstring pSoundKey, CHANNELID eID, _float fVolume, _bool bStop = false);
	void Play_BGM(wstring pSoundKey, _float fVolume, _bool bStop = false);
	void Stop_Sound(CHANNELID eID);
	void Stop_All();
	void Set_ChannelVolume(CHANNELID eID, float fVolume);
	_float Get_ChannelVolume(CHANNELID eID);

	FMOD_CHANNEL* Get_Channel(CHANNELID eID){ return m_pChannelArr[eID]; }
	const map<wstring, FMOD_SOUND*>& Get_MapSound() const { return m_mapSound; }

	const _int Get_SoundFileIndex(const wstring& pSoundKey);
	wstring Get_SoundFileKey(const _uint iIndex);
	wstring Get_SoundFileKey(string strKey);

private:
	void Search_Recursive(const std::string& currentPath);
	//void Load_SoundFile(const char* szSoundFilePath);
	

private:
	// ���� ���ҽ� ������ ���� ��ü 
	map<wstring, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	_float m_fSoundVolumeArr[MAXCHANNEL];

	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;

public:
	virtual void Free() override;

};

END