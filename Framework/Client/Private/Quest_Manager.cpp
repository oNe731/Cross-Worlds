#include "stdafx.h"
#include "Quest_Manager.h"

IMPLEMENT_SINGLETON(CQuest_Manager)

CQuest_Manager::CQuest_Manager()
{

}

HRESULT CQuest_Manager::Reserve_Manager(const wstring& strPatriclePath)
{
	/* ��������Ʈ�� ��������Ʈ���� �������. */

	return S_OK;
}

void CQuest_Manager::Tick(_float fTimeDelta)
{
}

void CQuest_Manager::Free()
{
}
