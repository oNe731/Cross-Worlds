#include "FileUtils.h"
#include "Utils.h"
#include "..\Public\JsonUtils.h"
#include <fstream>
#include <filesystem>

CJsonUtils::CJsonUtils()
{
}

CJsonUtils::~CJsonUtils()
{
	if (_handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(_handle);
		_handle = INVALID_HANDLE_VALUE;
	}
}

Json CJsonUtils::Json_Load(const wstring& strFilePath)
{
	Json data;
	
	_tchar szExt[MAX_PATH] = L"";
	_wsplitpath_s(strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);
	
	
	if (0 != lstrcmp(szExt, L".json"))
	{
		MSG_BOX("���� Ȯ���ڴ� .json �̾�� �մϴ�.");
		return data;
	}
	
	auto path = filesystem::path(strFilePath);
	
	filesystem::create_directories(path.parent_path());
	
	string strJson = "";
	
	/* ���⼭���� �߰��� �ڵ� */
	ifstream file(strFilePath, ios::binary);
	if (!file.is_open()) {
		MSG_BOX("������ �� �� �����ϴ�.");
		return data;
	}

	getline(file, strJson, static_cast<char>(EOF));

	// Bom�� �ִٸ� ���� �� �ε�.
	if (strJson.size() >= 3 && strJson[0] == '\xEF' && strJson[1] == '\xBB' && strJson[2] == '\xBF') {
		strJson.erase(0, 3);

		data = data.parse(strJson);
	}
	else
	{
		shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
		File->Open(strFilePath, FileMode::Read);

		strJson = File->Read<string>();

		data = data.parse(strJson);
	}

	return data;
}

HRESULT CJsonUtils::Json_Save(const wstring& strFilePath, const Json& refJsonFile)
{
	string strJson = refJsonFile.dump(1);

	_tchar szExt[MAX_PATH] = L"";
	_wsplitpath_s(strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);


	if (0 != lstrcmp(szExt, L".json"))
	{
		MSG_BOX("���� Ȯ���ڴ� .json �̾�� �մϴ�.");
		return E_FAIL;
	}

	auto path = filesystem::path(strFilePath);

	filesystem::create_directories(path.parent_path());


	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strFilePath, FileMode::Write);

	// BOM�� �Ϻ� �ؽ�Ʈ �����Ϳ��� ������ ���ڵ��� �ν��ϴµ� ���.
	// ��κ��� �ؽ��� ������ �� ���α׷��� UTF-8�� �� �ν��ϳ� �幰�� �ν��� ���ϴ� ��쵵 �־ BOM�� �߰�.
	 
	// UTF-8 BOM�� ��Ÿ���� 3����Ʈ�� ��.
	// \xEF: UTF - 8���� BOM�� ���� �κ��� ��Ÿ���� ����Ʈ.
	// \xBB : BOM�� �߰� �κ��� ��Ÿ���� ����Ʈ.
	// \xBF : BOM�� ������ �κ��� ��Ÿ���� ����Ʈ.
	// ������ BOM�� ���ԵǸ� ����ġ ���� ����� �ʷ��� �� �����Ƿ� ��� �� ����
	string bom = "\xEF\xBB\xBF"; 
	File->Write(bom.data(), bom.size());
	
	File->Write(strJson.data(), strJson.size());

	// File->Write<string>(strJson);

	
	return S_OK;
}

