#include "stdafx.h"
#include "Hammer.h"

#include "GameInstance.h"
#include "Collision_Manager.h"
#include "Weapon_Manager.h"

CHammer::CHammer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CWeapon(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_WEAPON)
{

}


HRESULT CHammer::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CHammer::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CHammer::LateTick(_float fTimeDelta)
{
	/* ������ ���ʰ� �ش� ���⸦ ����ϱ����� ���� ����� �޾ƿͼ� ������ Set_SocketWorld()�� ���־�� �մϴ�. */
	/* Set_SocketWorld()�� ���� ���� ��������� ��ȯ�մϴ�. (�ش� ��Ŀ� ������ ��������� ���ϸ�, ����󿡼��� ���� ��ġ�� �˴ϴ�.)*/

	__super::LateTick(fTimeDelta);
}

HRESULT CHammer::Render()
{
	if (FAILED(__super::Render() || nullptr == m_pModelCom))
		return E_FAIL;


	return S_OK;
}

void CHammer::Collision_Enter(const COLLISION_INFO& tInfo)
{

}

void CHammer::Collision_Continue(const COLLISION_INFO& tInfo)
{

}

void CHammer::Collision_Exit(const COLLISION_INFO& tInfo)
{

}

HRESULT CHammer::Ready_Trails()
{
	return S_OK;
}

HRESULT CHammer::Ready_Components()
{
	/* Com_Model */
	

	return S_OK;
}

HRESULT CHammer::Ready_Colliders()
{
	return S_OK;
}

HRESULT CHammer::Bind_ShaderResources()
{
	return S_OK;
}


CHammer* CHammer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHammer* pInstance = new CHammer(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CHammer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHammer::Clone(void* pArg)
{
	return nullptr;
}


void CHammer::Free()
{
	__super::Free();
}
