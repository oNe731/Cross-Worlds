#include "stdafx.h"
#include "UI_PlayerHPBar.h"
#include "GameInstance.h"

CUI_PlayerHPBar::CUI_PlayerHPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_PlayerHPBar")
{
}

CUI_PlayerHPBar::CUI_PlayerHPBar(const CUI_PlayerHPBar& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_PlayerHPBar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_PlayerHPBar::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	return S_OK;
}

void CUI_PlayerHPBar::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::Tick(fTimeDelta);
	}
}

void CUI_PlayerHPBar::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		// Todo : Player������ �����Ǹ� Set_Level�� ���� UIManager�� ��������.

		// ������
		CRenderer::TEXT_DESC  MAXHPDesc;
		MAXHPDesc.strText = L"/";
		MAXHPDesc.strFontTag = L"Default_Medium";
//		MAXHPDesc.vPosition = _float2(m_vDefaultPosition.x - 1.f, m_vDefaultPosition.y);
//		MAXHPDesc.vColor = _float4(0.f, 0.f, 0.f, 1.f);
		MAXHPDesc.vScale = { 0.4f, 0.4f };
//		m_pRendererCom->Add_Text(MAXHPDesc);
//		MAXHPDesc.vPosition = _float2(m_vDefaultPosition.x + 1.f, m_vDefaultPosition.y);
//		m_pRendererCom->Add_Text(MAXHPDesc);
//		MAXHPDesc.vPosition = _float2(m_vDefaultPosition.x, m_vDefaultPosition.y - 1.f);
//		m_pRendererCom->Add_Text(MAXHPDesc);
//		MAXHPDesc.vPosition = _float2(m_vDefaultPosition.x, m_vDefaultPosition.y + 1.f);
//		m_pRendererCom->Add_Text(MAXHPDesc);

		MAXHPDesc.vPosition = m_vDefaultPosition;
		MAXHPDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
		m_pRendererCom->Add_Text(MAXHPDesc);

		// Todo : ü���� �޾ƿ��Բ� ���� ���� �ʿ���.
		// ���� ü�� ���� �ܰ���
		CRenderer::TEXT_DESC CurHPDesc;
		CurHPDesc.strText = L"1234";
		CurHPDesc.strFontTag = L"Default_Medium";
//		CurHPDesc.vPosition = _float2(m_vCurHPPosition.x - 1.f, m_vCurHPPosition.y);
//		CurHPDesc.vColor = _float4(0.f, 0.f, 0.f, 1.f);
		CurHPDesc.vScale = { 0.4f, 0.4f };
//		m_pRendererCom->Add_Text(CurHPDesc);
//		CurHPDesc.vPosition = _float2(m_vCurHPPosition.x + 1.f, m_vCurHPPosition.y);
//		m_pRendererCom->Add_Text(CurHPDesc);
//		CurHPDesc.vPosition = _float2(m_vCurHPPosition.x, m_vCurHPPosition.y - 1.f);
//		m_pRendererCom->Add_Text(CurHPDesc);
//		CurHPDesc.vPosition = _float2(m_vCurHPPosition.x, m_vCurHPPosition.y + 1.f);
//		m_pRendererCom->Add_Text(CurHPDesc);

		CurHPDesc.vPosition = m_vCurHPPosition;
		CurHPDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
		m_pRendererCom->Add_Text(CurHPDesc);

		// �ִ� ü�� ���� �ܰ���
		CRenderer::TEXT_DESC MaxHPDesc;
		MaxHPDesc.strText = L"1235";
		MaxHPDesc.strFontTag = L"Default_Medium";
//		MaxHPDesc.vPosition = _float2(m_vMaxHPPosition.x - 1.f, m_vMaxHPPosition.y);
//		MaxHPDesc.vColor = _float4(0.f, 0.f, 0.f, 1.f);
		MaxHPDesc.vScale = { 0.4f, 0.4f };
//		m_pRendererCom->Add_Text(MaxHPDesc);
//		MaxHPDesc.vPosition = _float2(m_vMaxHPPosition.x + 1.f, m_vMaxHPPosition.y);
//		m_pRendererCom->Add_Text(MaxHPDesc);
//		MaxHPDesc.vPosition = _float2(m_vMaxHPPosition.x, m_vMaxHPPosition.y - 1.f);
//		m_pRendererCom->Add_Text(MaxHPDesc);
//		MaxHPDesc.vPosition = _float2(m_vMaxHPPosition.x, m_vMaxHPPosition.y + 1.f);
//		m_pRendererCom->Add_Text(MaxHPDesc);

		MaxHPDesc.vPosition = m_vMaxHPPosition;
		MaxHPDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
		m_pRendererCom->Add_Text(MaxHPDesc);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_PlayerHPBar::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_PlayerHPBar::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_PlayerHP_GaugeBar"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_PlayerHPBar::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_PlayerHPBar::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CUI_PlayerHPBar* CUI_PlayerHPBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_PlayerHPBar* pInstance = new CUI_PlayerHPBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_PlayerHPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PlayerHPBar::Clone(void* pArg)
{
	CUI_PlayerHPBar* pInstance = new CUI_PlayerHPBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_PlayerHPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PlayerHPBar::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
