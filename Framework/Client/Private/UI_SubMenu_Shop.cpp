#include "stdafx.h"
#include "UI_SubMenu_Shop.h"
#include "GameInstance.h"

CUI_SubMenu_Shop::CUI_SubMenu_Shop(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	const wstring& strObjectTag, UI_SUBSHOP_TYPE eUIType)
	: CUI(pDevice, pContext, strObjectTag)
	, m_eUIType(eUIType)
{
}

CUI_SubMenu_Shop::CUI_SubMenu_Shop(const CUI_SubMenu_Shop& rhs)
	: CUI(rhs)
	, m_eUIType(rhs.m_eUIType)
	, m_iTextureIndex(rhs.m_iTextureIndex)
{
}

void CUI_SubMenu_Shop::Set_Active(_bool bActive)
{
	if (SHOPMENU_END == m_eUIType)
		return;

	if (bActive)
	{
		if (m_bUsable)
			m_fAlpha = 0.4f;

		m_tInfo.fY = m_vOnPosition.y;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

		m_bUpdateAlpha = false;
		m_bArrived = false;
	}
	else
	{
		if (m_bEvent)
			m_bEvent = false;
	}

	m_bActive = bActive;
}

HRESULT CUI_SubMenu_Shop::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SubMenu_Shop::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	// ���� ��ġ�� ��ǥ�� �������ش�.

	if (SHOPMENU_END == m_eUIType)
		return E_FAIL;


	// ù��° ���� �����Ǵ� Btn -> ������ ��ǥ SHOPBTN_PACKAGE
	m_vOnPosition.x = g_iWinSizeX - 230.f;
	m_vOnPosition.y = 370.f;

	// ��ǥ ��ġ�� �ʱ� ������ġ�̴�.
	m_vGoalPosition.x = m_tInfo.fX;
	m_vGoalPosition.y = m_tInfo.fY; // Y���� �����

	m_bActive = false;

	return S_OK;
}

void CUI_SubMenu_Shop::Tick(_float fTimeDelta)
{
	if (SHOPMENU_END == m_eUIType)
		return;

	if (m_bActive)
	{
		if (m_bArrived)
		{
			m_tInfo.fY = m_vGoalPosition.y;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
		}
		else
		{
			if (m_tInfo.fY > m_vGoalPosition.y)
				m_bArrived = true;

			m_tInfo.fY += fTimeDelta * m_fSpeed;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

		}
		__super::Tick(fTimeDelta);
	}
}

void CUI_SubMenu_Shop::LateTick(_float fTimeDelta)
{
	if (SHOPMENU_END == m_eUIType)
		return;

	if (m_bActive)
	{
		if (m_bUsable)
		{
			if (!m_bUpdateAlpha)
			{
				// ���İ��� �����Ѵ�.
				if (m_fAlpha < 0.9f)
				{
					m_fAlpha += fTimeDelta;
				}
				else if (0.9f <= m_fAlpha)
				{
					m_bUpdateAlpha = true;
					m_fAlpha = 0.9f;
				}
			}
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_SubMenu_Shop::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_SubMenu_Shop::On_MouseEnter(_float fTimeDelta)
{
	if (m_bActive)
	{
	}
}

void CUI_SubMenu_Shop::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			GI->Stop_Sound(CHANNELID::SOUND_UI2);
			GI->Play_Sound(TEXT("UI_Fx_Comm_Btn_1.mp3"), CHANNELID::SOUND_UI2, GI->Get_ChannelVolume(CHANNELID::SOUND_UI2));
		}

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_SubMenu_Shop::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

void CUI_SubMenu_Shop::Reset_InitializeInfo()
{
//	if (BTN_CLICKED == m_eTextureType)
//	{
//		m_bActive = false;
//		m_bClicked = false;
//		m_bArrived = false;
//		m_bMoveStart = false;
//		m_bMoveEnd = false;
//	}
}

HRESULT CUI_SubMenu_Shop::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	Ready_Buttons();
	
	if (m_bUsable) // ����� �����ܵ� -> alpha�� ������ ���� ���̴�.
	{
		m_fAlpha = 0.4f;
	}
	else // ������� ���� �����ܵ� -> alpha���� �����ȴ�.
	{
		m_fAlpha = 0.3f;
	}

	return S_OK;
}

HRESULT CUI_SubMenu_Shop::Ready_Buttons()
{
	_float fStandard = 300.f;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SubMenu_Shop"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	switch (m_eUIType)
	{
	case SHOPBTN_PACKAGE:
		m_iTextureIndex = 0;
		m_fSpeed = 0.f;
		m_bUsable = true;
		break;
	case SHOPBTN_GACHA:
		m_iTextureIndex = 1;
		m_fSpeed = fStandard;
		m_bUsable = false;
		break;
	case SHOPBTN_TOKEN:
		m_iTextureIndex = 2;
		m_fSpeed = fStandard * 2.f;
		m_bUsable = false;
		break;
	}

	return S_OK;
}

HRESULT CUI_SubMenu_Shop::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_SubMenu_Shop::Bind_ShaderResources()
{
	if (m_iTextureIndex < 0 || 2 < m_iTextureIndex)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	return S_OK;
}

CUI_SubMenu_Shop* CUI_SubMenu_Shop::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	const wstring& strObjectTag, UI_SUBSHOP_TYPE eUIType)
{
	CUI_SubMenu_Shop* pInstance = new CUI_SubMenu_Shop(pDevice, pContext, strObjectTag, eUIType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_SubMenu_Shop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SubMenu_Shop::Clone(void* pArg)
{
	CUI_SubMenu_Shop* pInstance = new CUI_SubMenu_Shop(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_SubMenu_Shop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SubMenu_Shop::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
