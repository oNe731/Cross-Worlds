#include "stdafx.h"
#include "GameInstance.h"
#include "Inventory_Manager.h"
#include "GameItem.h"

#include "GameItem_Consumpsion.h"

IMPLEMENT_SINGLETON(CInventory_Manager)

CInventory_Manager::CInventory_Manager()
{

}

HRESULT CInventory_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	for (auto& iter : m_Inventory)
		iter.reserve(8);

	return S_OK;
}

HRESULT CInventory_Manager::Ready_Items()
{
	// �������� ������Ÿ���� �����Ѵ�.

//	CGameItem::ITEM_DESC ItemDesc = {};
//	ItemDesc.eCode = ITEM_CODE::CONSUMPSION_ENERGY;
//	ItemDesc.eMainCategory = CGameItem::CATEGORY_POTION;
//	ItemDesc.strName = TEXT("������ �帵ũ");
//	ItemDesc.strContent = TEXT("��� �� 30�� ���� �¸��� �������� ���� ����. \n �Ϲ� �ʵ忡�� ����� ���� ����ġ/��� ȹ�淮�� 100& �����ؿ�.");
//	
//	m_Inventory[INVEN_CONSUMPSION].push_back(CGameItem_Consumpsion::Create(&ItemDesc));

	return S_OK;
}

void CInventory_Manager::Tick(_float fTimeDelta)
{
}

void CInventory_Manager::LateTick(_float fTimeDelta)
{
}

HRESULT CInventory_Manager::Add_Item(CGameObject* pItem)
{
	//CGameItem* pItem = dynamic_cast<CGameItem*>(pItem);
	//if(pItem->Get_ItemType())
	return S_OK;
}

_bool CInventory_Manager::Is_InInventory(CGameObject* pItem)
{
	return _bool();
}

void CInventory_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}




