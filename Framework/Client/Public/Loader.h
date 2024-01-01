#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* 1. ������������ ���� �ε� �۾��� ���٤�������. */

BEGIN(Client)

class CLoader final : public CBase
{

private:
	enum LOADING_THREAD { 
		STATIC_OBJECT_PROTOTYPE, 
		DYNAMIC_OBJECT_PROTOTYPE, 
		LOAD_MAP,
		MONSTER_AND_NPC,
		CHARACTER_MODEL_SWORDMAN, 
		CHARACTER_MODEL_ENGINEER,
		CHARACTER_MODEL_DESTROYER,
		THREAD_END 
	};
private:
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual~CLoader() = default;

public:
	const wstring Get_LoadingText() const {
		return m_strLoading;
	}

	_bool Get_Finished() const {
		return m_isFinished;
	}

public:
	/* eNextLevel�� ���� �ε��� �ϱ����� �����带 �����Ѵ�. */
	HRESULT Initialize(LEVELID eNextLevel, const wstring& strFolderName);
	_int	Loading();

public:
	static _bool g_bFirstLoading;


private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	LEVELID					m_eNextLevel = { LEVEL_END };
	wstring					m_strFolderName = { L"" };
	wstring					m_strLoading = { TEXT("") };
	_bool					m_isFinished = { false };

private:
	HANDLE					m_hThread = { 0 };
	CRITICAL_SECTION		m_Critical_Section;



private:
	HRESULT Loading_For_Level_Logo();
	HRESULT Loading_For_Level_Lobby();
	HRESULT Loading_For_Level_Evermore();
	HRESULT Loading_For_Level_Test();
	HRESULT Loading_For_Level_Tool();
	HRESULT Load_Navi_Data(const wstring& strNaviFileName);
	HRESULT Load_Map_Data(const wstring& strMapFileName);
	HRESULT Load_Monster_Data(const wstring& strMonsterFileName);

private:
	// ������ ����� ��� fbx ���� ��ü�� �ε��Ѵ�.
	HRESULT Loading_Proto_Static_Map_Objects(const wstring& strPath);
	HRESULT Loading_Proto_Dynamic_Map_Objects(const wstring& strPath);
	HRESULT Loading_Proto_Monster_Npc();


	// ĳ���� �ε� �� �Ŵ��� �غ�.
	HRESULT Loading_For_Character(CHARACTER_TYPE eCharacterType);
	HRESULT Loading_Character_Models(const wstring& strFolderPath);
	HRESULT Reserve_Character_Managers();


private:
	std::future<HRESULT> m_Threads[THREAD_END];

public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevel, const wstring& strFolderName);
	virtual void Free() override;
};

END 