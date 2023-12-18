#include "stdafx.h"
#include "UI_Loading_CharacterLogo.h"
#include "GameInstance.h"

CUI_Loading_CharacterLogo::CUI_Loading_CharacterLogo(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI(pDevice, pContext, L"UI_Loading_CharacterLogo")
{
	ZeroMemory(m_szInfoText, sizeof(TCHAR) * MAX_PATH);
}

CUI_Loading_CharacterLogo::CUI_Loading_CharacterLogo(const CUI_Loading_CharacterLogo& rhs)
	: CUI(rhs)
{
	//lstrcpy(m_szInfoText, rhs.m_szInfoText);
}

HRESULT CUI_Loading_CharacterLogo::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading_CharacterLogo::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_fAlpha = 0.7f;
	//m_iTextureIndex = 4; // Temp

	//Set_Text();

	return S_OK;
}

void CUI_Loading_CharacterLogo::Tick(_float fTimeDelta)
{
	if (m_iTextureIndex < 0 || 4 < m_iTextureIndex)
		return;

	__super::Tick(fTimeDelta);

	Tick_Text(fTimeDelta);
}

void CUI_Loading_CharacterLogo::LateTick(_float fTimeDelta)
{
	if (m_iTextureIndex < 0 || 4 < m_iTextureIndex)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_Loading_CharacterLogo::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	if (0 != m_iTextCount)
	{
		TCHAR sTempText[MAX_PATH];
		ZeroMemory(sTempText, sizeof(TCHAR) * MAX_PATH);
		for (_uint i = 0; i < m_iTextCount; ++i)
		{
			sTempText[i] = m_szInfoText[i];
		}

		CRenderer::TEXT_DESC  MAXHPDesc;
		MAXHPDesc.strText = sTempText;
		MAXHPDesc.strFontTag = L"Default_Medium";
		MAXHPDesc.vScale = m_vScale;
		MAXHPDesc.vPosition = m_vPosition;
		MAXHPDesc.vColor = m_vColor;
		m_pRendererCom->Add_Text(MAXHPDesc);
	}

	return S_OK;
}

HRESULT CUI_Loading_CharacterLogo::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// Texture Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_CharacterLogos"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Loading_CharacterLogo::Ready_State()
{
	m_tInfo.fCX = 800.f * 0.5f;
	m_tInfo.fCY = 102.f * 0.5f;
	m_tInfo.fX = (g_iWinSizeX * 0.5f) - 475.f; // - 60px
	m_tInfo.fY = g_iWinSizeY * 0.5f - 250.f;

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Loading_CharacterLogo::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CUI_Loading_CharacterLogo::Set_Text()
{
//	if (!lstrcmp(m_szInfoText, pText))
//		return;
//
//	ZeroMemory(m_szInfoText, sizeof(TCHAR) * MAX_PATH);
//	lstrcpy(m_szInfoText, pText);

	switch (m_iTextureIndex)
	{
	case 0:
		_tcscpy_s(m_szInfoText, _T("�Ѽհ��� �̿��� ������ ������ ������ �ڶ��ϴ�\nȣ���� ������ �˻�\n���������� ��ġ�� ȭ���� �˼���\n������ ��⸦ �����ϴ�."));
		m_vColor = _float4(0.027f, 0.157f, 0.204f, 1.f);
		break;

	case 1:
		_tcscpy_s(m_szInfoText, _T("�Ŵ��� ��ġ�� ������ �ϰ��� ������ ��ģ �ŷ��� ����\n�����Դ� ���ں�������\n�Ʊ����Դ� �������ٵ� �������� �����Դϴ�."));
		m_vColor = _float4(0.176f, 0.141f, 0.106f, 1.f);
		break;

	case 2:
		_tcscpy_s(m_szInfoText, _T("�������� ����� ���� ȭ�⸦ �ٷ�� ���� ������\n������ ������� ȸ�������ִ�\n������ õ���̱⵵ �մϴ�."));
		m_vColor = _float4(0.059f, 0.157f, 0.153f, 1.f);
		break;

	case 3:
		_tcscpy_s(m_szInfoText, _T("������ ��� â�� �Բ� ��Ȥ�� ���� �ߴ�\n�Ϻ������� ������\n�پ��� ȿ���� ���� ��ų�ε�\n��� ��Ȳ�� �����մϴ�."));
		m_vColor = _float4(0.498f, 0.329f, 0.408f, 1.f);
		break;

	case 4:
		_tcscpy_s(m_szInfoText, _T("��ø�ϰ� �����̸� Ȱ�� ���� �����ϴ� ���Ÿ� ����\n��Ƽ�� ���� ���ϰ� ������ִ�\n����� ������ ���ҵ� �����մϴ�."));
		m_vColor = _float4(0.588f, 0.494f, 0.235f, 1.f);
		break;
	}

	wstring sTemp = m_szInfoText;
	m_iMaxCount = sTemp.size();
	m_iTextCount = 0;
	m_fTimeAcc = 0.f;
}

void CUI_Loading_CharacterLogo::Tick_Text(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (0.1f < m_fTimeAcc)
	{
		++m_iTextCount;
		if (MAX_PATH <= m_iTextCount)
			m_iTextCount = MAX_PATH - 1;

		m_fTimeAcc = 0.f;
	}
}

CUI_Loading_CharacterLogo * CUI_Loading_CharacterLogo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Loading_CharacterLogo*	pInstance = new CUI_Loading_CharacterLogo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Loading_CharacterLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_Loading_CharacterLogo::Clone(void* pArg)
{
	CUI_Loading_CharacterLogo*	pInstance = new CUI_Loading_CharacterLogo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Loading_CharacterLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Loading_CharacterLogo::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
