#include "stdafx.h"
#include "UI_SkillSection_Background.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_SkillSection_Background::CUI_SkillSection_Background(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_SkillSection_Background")
{
}

CUI_SkillSection_Background::CUI_SkillSection_Background(const CUI_SkillSection_Background& rhs)
	: CUI(rhs)
{
}

void CUI_SkillSection_Background::Set_Active(_bool bActive)
{
	if (bActive)
	{
		for (auto& pChildUI : m_pChild)
		{
			if (nullptr != pChildUI)
				pChildUI->Set_Active(true);
		}
	}
	else
	{
		for (auto& pChildUI : m_pChild)
		{
			if (nullptr != pChildUI)
				pChildUI->Set_Active(false);
		}
	}

	m_bActive = bActive;
}

HRESULT CUI_SkillSection_Background::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SkillSection_Background::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true; 
	m_fAlpha = 0.9f;

	_float fSize = 64.f * 0.8f;

	// �÷��̾� ���¸� ��Ʈ���ϴ� ��ư��
	Make_Child(71.f, -77.f, fSize, fSize, TEXT("Prototype_GameObject_UI_SkillSection_BtnJump"));
	Make_Child(-77.f, 71.f, fSize, fSize, TEXT("Prototype_GameObject_UI_SkillSection_BtnRoll"));
	
	// ���� ����
	_float2 vDefaultOffset = _float2(25.f, -195.f);
	_float fSlotSize = 128.f * 0.6f;
	_float fOffset = 85.f;

	Make_Child(vDefaultOffset.x - fOffset, vDefaultOffset.y, fSlotSize, fSlotSize, TEXT("Prototype_GameObject_UI_WeaponSection_Slot_First"));
	Make_Child(vDefaultOffset.x, vDefaultOffset.y, fSlotSize, fSlotSize, TEXT("Prototype_GameObject_UI_WeaponSection_Slot_Second"));
	Make_Child(vDefaultOffset.x + fOffset, vDefaultOffset.y, fSlotSize, fSlotSize, TEXT("Prototype_GameObject_UI_WeaponSection_Slot_Third"));

	// �⺻ ��ų
	_float fBtnSize = 128.f * 0.4f;
	Make_Child(-5.f, -1.f, fBtnSize, fBtnSize, TEXT("Prototype_GameObject_UI_SkillSection_DefaultAttack"));

	// ��ȣ�ۿ� ��ư
	fBtnSize = 102.f * 0.7f;
	Make_Child(-170.f, -80.f, fBtnSize, fBtnSize, TEXT("Prototype_GameObject_UI_SkillSection_BtnInteraction"));

	fBtnSize = 128.f * 0.5f;
	Make_Child(-170.f, -80.f, fBtnSize, fBtnSize, TEXT("Prototype_GameObject_UI_SkillSection_Interaction"));
	
	return S_OK;
}

void CUI_SkillSection_Background::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		__super::Tick(fTimeDelta);
	}
}

void CUI_SkillSection_Background::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta); // ChilUI�� ������ LateTick�� ��������.
	}
}

HRESULT CUI_SkillSection_Background::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();

		__super::Render();
	}

	return S_OK;
}

HRESULT CUI_SkillSection_Background::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillSection_Background"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_SkillSection_Background::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_SkillSection_Background::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

CUI_SkillSection_Background* CUI_SkillSection_Background::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_SkillSection_Background* pInstance = new CUI_SkillSection_Background(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_SkillSection_Background");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SkillSection_Background::Clone(void* pArg)
{
	CUI_SkillSection_Background* pInstance = new CUI_SkillSection_Background(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_SkillSection_Background");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SkillSection_Background::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
