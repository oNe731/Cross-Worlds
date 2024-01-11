#include "stdafx.h"
#include "Rifle.h"

#include "GameInstance.h"
#include "Collision_Manager.h"
#include "Weapon_Manager.h"

CRifle::CRifle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CWeapon(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_WEAPON)
{

}


HRESULT CRifle::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CRifle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CRifle::LateTick(_float fTimeDelta)
{
	/* ������ ���ʰ� �ش� ���⸦ ����ϱ����� ���� ����� �޾ƿͼ� ������ Set_SocketWorld()�� ���־�� �մϴ�. */
	/* Set_SocketWorld()�� ���� ���� ��������� ��ȯ�մϴ�. (�ش� ��Ŀ� ������ ��������� ���ϸ�, ����󿡼��� ���� ��ġ�� �˴ϴ�.)*/

	__super::LateTick(fTimeDelta);
}

HRESULT CRifle::Render()
{
	if (FAILED(__super::Render() || nullptr == m_pModelCom))
		return E_FAIL;


	return S_OK;
}

void CRifle::Collision_Enter(const COLLISION_INFO& tInfo)
{

}

void CRifle::Collision_Continue(const COLLISION_INFO& tInfo)
{

}

void CRifle::Collision_Exit(const COLLISION_INFO& tInfo)
{

}

HRESULT CRifle::Ready_Components()
{
	/* Com_Model */
	

	return S_OK;
}

HRESULT CRifle::Ready_Colliders()
{
	return S_OK;
}

HRESULT CRifle::Bind_ShaderResources()
{
	return S_OK;
}


CRifle* CRifle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CRifle* pInstance = new CRifle(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CRifle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRifle::Clone(void* pArg)
{
	return nullptr;
}


void CRifle::Free()
{
	__super::Free();
}
