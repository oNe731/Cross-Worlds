#include "stdafx.h"
#include "UI.h"
#include "GameInstance.h"



CUI::CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_UI)
{
	ZeroMemory(&m_tInfo, sizeof(UI_INFO));
}

CUI::CUI(const CUI& rhs)
	: CGameObject(rhs)
	, m_tInfo(rhs.m_tInfo)
	, m_strText(rhs.m_strText)
{
}

HRESULT CUI::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		m_tInfo = *((UI_INFO*)pArg);
	}
	else
	{
		//Arg를 넣어주지 않은 상황이라면 기본 세팅으로 생성한다.
		m_tInfo.fX = g_iWinSizeX * 0.5f;
		m_tInfo.fY = g_iWinSizeY * 0.5f;
		m_tInfo.fCX = _float(g_iWinSizeX);
		m_tInfo.fCY = _float(g_iWinSizeY);
	}

	// View행렬과 Proj행렬을 채워준다.
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI::Tick(_float fTimeDelta)
{
	POINT pt = GI->GetMousePos();
	RECT MouseRect;
	MouseRect.left = pt.x - 1.f;
	MouseRect.right = pt.x + 1.f;
	MouseRect.top = pt.y - 1.f;
	MouseRect.bottom = pt.y + 1.f;

	RECT UIRect;
	UIRect.left = m_tInfo.fX - m_tInfo.fCX / 2.f;
	UIRect.right = m_tInfo.fX + m_tInfo.fCX / 2.f;
	UIRect.top = m_tInfo.fY - m_tInfo.fCY / 2.f;
	UIRect.bottom = m_tInfo.fY + m_tInfo.fCY / 2.f;

	RECT Result;
	if (IntersectRect(&Result, &MouseRect, &UIRect))
	{
		// 들어와 있다.
		switch(m_eMouseState)
		{
		case UI_MOUSESTATE::MOUSE_ENTER:
			m_eMouseState = UI_MOUSESTATE::MOUSE_ON;
			On_Mouse(fTimeDelta);
			break;
		case UI_MOUSESTATE::MOUSE_ON:
			On_Mouse(fTimeDelta);
			break;
		case UI_MOUSESTATE::MOUSE_EXIT:
			m_eMouseState = UI_MOUSESTATE::MOUSE_ENTER;
			On_MouseEnter(fTimeDelta);
			break;
		default:
			m_eMouseState = UI_MOUSESTATE::MOUSE_ENTER;
			On_MouseEnter(fTimeDelta);
			break;

		}
	}
	else
	{
		// 안들어와 있다.
		switch (m_eMouseState)
		{
		case UI_MOUSESTATE::MOUSE_ENTER:
			m_eMouseState = UI_MOUSESTATE::MOUSE_EXIT;
			On_MouseExit(fTimeDelta);
			break;
		case UI_MOUSESTATE::MOUSE_ON:
			m_eMouseState = UI_MOUSESTATE::MOUSE_EXIT;
			On_MouseExit(fTimeDelta);
			break;
		case UI_MOUSESTATE::MOUSE_EXIT:
			m_eMouseState = UI_MOUSESTATE::MOUSE_END;
			break;
		default:
			break;
		}
	}

}

void CUI::Debug_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LEFT_ARROW))
		m_tInfo.fX -= 10.f;

	if(KEY_TAP(KEY::RIGHT_ARROW))
		m_tInfo.fX += 10.f;

	if (KEY_TAP(KEY::UP_ARROW))
		m_tInfo.fY += 10.f;

	if (KEY_TAP(KEY::DOWN_ARROW))
		m_tInfo.fY -= 10.f;

	if (KEY_TAP(KEY::OPEN_SQUARE_BRACKET))
		m_tInfo.fCX -= 10.f;

	if (KEY_TAP(KEY::CLOSE_SQUARE_BRACKET))
		m_tInfo.fCX += 10.f;

	if(KEY_TAP(KEY::O))
		m_tInfo.fCY -= 10.f;

	if (KEY_TAP(KEY::P))
		m_tInfo.fCY += 10.f;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -m_tInfo.fY + g_iWinSizeY * 0.5f, 1.f, 1.f));
	m_pTransformCom->Set_Scale(XMLoadFloat3(&_float3(m_tInfo.fCX, m_tInfo.fCY, 1.f)));
}

HRESULT CUI::Ready_Components()
{
	// Transform Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// Renderer Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// Shader Coomponent
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// VIBuffer Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

void CUI::Free()
{
	__super::Free();

//	for (auto& pUI : m_pChild)
//		Safe_Release(pUI);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);

}

//  m_pTransformCom->Set_State(CTransform::STATE_POSITION,
//	XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));