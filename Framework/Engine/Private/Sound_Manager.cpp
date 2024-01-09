#include "Sound_Manager.h"
#include <io.h>
#include <tchar.h>
#include <filesystem>

#include "Utils.h"

namespace fs = std::filesystem;

IMPLEMENT_SINGLETON(CSound_Manager)

CSound_Manager::CSound_Manager()
	: m_pSystem(nullptr)
{

}

CSound_Manager::~CSound_Manager()
{
	Free();
}

HRESULT CSound_Manager::Reserve_Manager()
{
	FMOD_System_Create(&m_pSystem);
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	for (_uint i = 0; i < MAXCHANNEL; ++i)
	{
		m_fSoundVolumeArr[i] = 1.f;
	}
	
	const string topLevelPath = "../../Client/Bin/Resources/Sound/";

	if (fs::exists(topLevelPath) && fs::is_directory(topLevelPath)) 
		Search_Recursive(topLevelPath);

	/*Load_SoundFile("../../Client/Bin/Resources/Sound/Bgm/");
	Load_SoundFile("../../Client/Bin/Resources/Sound/Battle/");
	Load_SoundFile("../../Client/Bin/Resources/Sound/Foot/");
	Load_SoundFile("../../Client/Bin/Resources/Sound/Skill/");
	Load_SoundFile("../../Client/Bin/Resources/Sound/Slash/");
	Load_SoundFile("../../Client/Bin/Resources/Sound/UI/");
	Load_SoundFile("../../Client/Bin/Resources/Sound/Voice/");*/
	

	return S_OK;
}


void CSound_Manager::Play_Sound(wstring pSoundKey, CHANNELID eID, _float fVolume, _bool bStop)
{
	map<wstring, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
		{
			return pSoundKey == iter.first;
		});

	if (iter == m_mapSound.end())
		return;

	if (true == bStop)
		Stop_Sound(eID);

	FMOD_BOOL bPlay = FALSE;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
	FMOD_System_Update(m_pSystem);
}


void CSound_Manager::Play_BGM(wstring pSoundKey, _float fVolume, _bool bStop)
{
	map<wstring, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
		{
			return pSoundKey == iter.first;
		});

	if (iter == m_mapSound.end())
		return;

	if (true == bStop)
		Stop_Sound(SOUND_BGM);

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[SOUND_BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[SOUND_BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[SOUND_BGM], fVolume);
	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::Stop_Sound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSound_Manager::Stop_All()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSound_Manager::Set_ChannelVolume(CHANNELID eID, float fVolume)
{
	m_fSoundVolumeArr[eID] = fVolume;
	FMOD_Channel_SetVolume(m_pChannelArr[eID], m_fSoundVolumeArr[eID]);
	FMOD_System_Update(m_pSystem);
}

_float CSound_Manager::Get_ChannelVolume(CHANNELID eID)
{
	if (CHANNELID::MAXCHANNEL < eID)
		return 0.f;

	return 	m_fSoundVolumeArr[eID];;
}

const _int CSound_Manager::Get_SoundFileIndex(const wstring& pSoundKey)
{
	_int iIndex = 0;
	for (auto& Pair : m_mapSound)
	{
		if (Pair.first == pSoundKey)
			return iIndex;
		else
			iIndex++;
	}

	return -1;
}

wstring CSound_Manager::Get_SoundFileKey(const _uint iIndex)
{
	_int iKeyIndex = 0;
	for (auto& Pair : m_mapSound)
	{
		if (iIndex == iKeyIndex)
			return Pair.first;
		else
			iKeyIndex++;
	}

	return L"";
}

wstring CSound_Manager::Get_SoundFileKey(string strKey)
{
	for (auto& Pair : m_mapSound)
	{
		if (CUtils::ToString(Pair.first) == strKey)
			return Pair.first;
	}
	return L"";
}

void CSound_Manager::Search_Recursive(const std::string& currentPath)
{
	for (const auto& entry : fs::directory_iterator(currentPath)) 
	{
		if (fs::is_directory(entry)) // �ֻ��� ��ο��� ��� ���� ������ Ž���Ѵ�.
		{
			Search_Recursive(entry.path().string());
		}
		else if (fs::is_regular_file(entry)) // �����̶�� ���� ���ҽ��� �����. 
		{
			char szFullPath[256] = "";
			strcpy_s(szFullPath, sizeof(char) * 128, entry.path().string().c_str());

			std::string fileName = fs::path(szFullPath).filename().stem().string() + fs::path(szFullPath).extension().string();

			FMOD_SOUND* pSound = nullptr;

			FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);

			if (eRes == FMOD_OK)
			{
				int iLength = fileName.length() + 1;

				wstring pSoundKey = CUtils::ToWString(fileName);

				m_mapSound.emplace(pSoundKey, pSound);
			}
		}
	}
}

//void CSound_Manager::Load_SoundFile(const char* szSoundFilePath)
//{
//	_finddata_t fd;
//
//	string Path = szSoundFilePath;
//	string Exe = "*.*";
//	string FullPath = Path + Exe;
//
//	// _findfirst : <io.h>���� �����ϸ� ����ڰ� ������ ��� ������ ���� ù ��° ������ ã�� �Լ�
//	intptr_t handle = _findfirst(FullPath.c_str(), &fd);
//
//	if (handle == -1)
//		return;
//
//	int iResult = 0;
//
//	char	szCurPath[128];
//	strcpy_s(szCurPath, sizeof(char) * 128, Path.c_str());
//
//	char szFullPath[128] = "";
//
//	while (iResult != -1)
//	{
//		strcpy_s(szFullPath, szCurPath);
//
//		// "../ Sound/Success.wav"
//		strcat_s(szFullPath, fd.name);
//		
//		FMOD_SOUND* pSound = nullptr;
//
//		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);
//
//		if (eRes == FMOD_OK)
//		{
//			int iLength = strlen(fd.name) + 1;
//
//			wstring pSoundKey = new TCHAR[iLength];
//			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
//
//			// �ƽ�Ű �ڵ� ���ڿ��� �����ڵ� ���ڿ��� ��ȯ�����ִ� �Լ�
//			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);
//
//			m_mapSound.emplace(pSoundKey, pSound);
//		}
//		//_findnext : <io.h>���� �����ϸ� ���� ��ġ�� ������ ã�� �Լ�, ���̻� ���ٸ� -1�� ����
//		iResult = _findnext(handle, &fd);
//	}
//
//	FMOD_System_Update(m_pSystem);
//	_findclose(handle);
//}

void CSound_Manager::Free()
{
	for (auto& Mypair : m_mapSound)
	{
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}
