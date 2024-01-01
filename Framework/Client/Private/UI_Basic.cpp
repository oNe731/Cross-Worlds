#include "stdafx.h"
#include "UI_Basic.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Game_Manager.h"

CUI_Basic::CUI_Basic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, UI_BASIC eType)
	: CUI(pDevice, pContext, strObjectTag), m_eType(eType)
{
}

CUI_Basic::CUI_Basic(const CUI_Basic& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_Basic::Set_Active(_bool bActive)
{
	if (UIQUEST_ACCEPT == m_eType || UIQUEST_FINISH == m_eType)
	{
		m_bActive = bActive;
	}
//	else if (COSTUME_INSTALL == m_eType)
//	{
//		CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
//		if (nullptr == pPlayer)
//			return;
//
//		CCharacter* pCharacter = pPlayer->Get_Character();
//
//		pCharacter->Get_PartModel(PART_TYPE::HEAD);
//		pCharacter->Get_PartModel(PART_TYPE::BODY);
//	}
	else
	{
		m_bActive = bActive;
	}
}

HRESULT CUI_Basic::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Basic::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (UIMAPNAME_EVERMORE == m_eType || UIMAPNAME_KINGDOM == m_eType ||
		UIMAPNAME_ICELAND == m_eType || UIMAPNAME_FOREST == m_eType)
	{
		// ���İ�, Active������ �ʿ��� Type
		m_bActive = false;
		m_bFade = true;
	}
	else if (UIQUEST_ACCEPT == m_eType || UIQUEST_FINISH == m_eType || COSTUME_INSTALL == m_eType)
	{
		m_bActive = false;
	}
	else if (UILOBBY_DICE == m_eType)
		Ready_Nickname();

	return S_OK;
}

void CUI_Basic::Tick(_float fTimeDelta)
{
	if (UIBASIC_END == m_eType)
		return;

	if (m_bActive)
	{
		if (m_bFade)
		{
			Tick_FadeObject(fTimeDelta);
		}
		else
		{
			//if (CUI_Basic::UILOBBY_BTNTEXT == m_eType)
			//{
				//		m_tInfo.fCX = 188.f * 0.7f;
				//		m_tInfo.fCY = 53.f * 0.7f;
				//		m_tInfo.fX = g_iWinSizeX - m_tInfo.fCX * 0.5f - 115.f;
				//		m_tInfo.fY = g_iWinSizeY - 70.f;
				//
				//		m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
				//		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				//			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
			//}
		}

		if (m_eType == UILOBBY_ANNOUNCE)
		{
			if (!m_bAlpha)
			{
				if (m_fAlpha <= 0.7f)
				{
					m_fAlpha -= fTimeDelta * 0.5f;

					if (m_fAlpha <= 0.f)
					{
						m_bAlpha = true;
						m_fAlpha = 0.f;
					}
				}
			}
			else
			{
				m_fAlpha += fTimeDelta * 0.5f;

				if (m_fAlpha >= 0.7f)
				{
					m_bAlpha = false;
					m_fAlpha = 0.7f;
				}
			}
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Basic::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_bFade)
		{
			LateTick_FadeObject(fTimeDelta);
		}
		else
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		if (UILOBBY_NICKFRAME == m_eType)
		{
			m_strText = CGame_Manager::GetInstance()->Get_UserName();
			// �۾��� �߰��Ѵ�.
			CRenderer::TEXT_DESC NickDesc = {};
			NickDesc.strText = m_strText;
			_int iLength = m_strText.length();

			NickDesc.strFontTag = L"Default_Bold";
			NickDesc.vScale = { 0.6f, 0.6f };
			NickDesc.vPosition = _float2(m_tInfo.fX - (iLength * 11.f), m_tInfo.fY - 5.f);
			NickDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
			m_pRendererCom->Add_Text(NickDesc);
		}

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Basic::Render()
{
	if (m_bActive)
	{
		if (m_bFade)
		{
			if (FAILED(Bind_ShaderResources()))
				return E_FAIL;

			m_pShaderCom->Begin(1);

			m_pVIBufferCom->Render();

			return S_OK;
		}
		else
		{
			if (FAILED(Bind_ShaderResources()))
				return E_FAIL;

			m_pShaderCom->Begin(m_iPass);

			m_pVIBufferCom->Render();

			return S_OK;
		}

		__super::Render();
	}

	return S_OK;
}

void CUI_Basic::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_Basic::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (UILOBBY_DICE == m_eType)
		{
			if (KEY_TAP(KEY::LBTN))
				Set_RandomNickname();
		}
	}
}

void CUI_Basic::On_MouseExit(_float fTimeDelta)
{
}

HRESULT CUI_Basic::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case UILOBBY_NICKFRAME: // Enum�� LEVEL_LOBBY
		if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_NicknameFrame"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 1.f;
		break;

	case UILOBBY_TITLE:
		if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_TitleText"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.9f;
		break;

	case UILOBBY_BTNTEXT:
		if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_BtnText"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.9f;
		break;

	case UILOBBY_ANNOUNCE:
		if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_Announce"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.7f;
		m_bAlpha = false;
		break;

	case UILOBBY_DICE:
		if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Lobby_SetNickname_Dice"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 1.f;
		break;

	case UIMAPNAME_EVERMORE:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MapName_Evermore"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.9f;
		break;

	case UIMAPNAME_KINGDOM:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MapName_KingdomHall"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.9f;
		break;

	case UIMAPNAME_ICELAND:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MapName_IceLand"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.9f;
		break;

	case UIMAPNAME_FOREST:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MapName_WitchForest"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.9f;
		break;

	case UIQUEST_ACCEPT:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Text_QuestAccept"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.9f;
		break;

	case UIQUEST_FINISH:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Text_QuestFinish"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.9f;
		break;

	case TARGETARROW_LEFT:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Monster_WorldHP_TargetArrow_Left"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 1.f;
		break;

	case TARGETARROW_RIGHT:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Monster_WorldHP_TargetArrow_Right"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 1.f;
		break;

	case COSTUME_INSTALL:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Announce_Install"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 1.f;
		break;

	case UISTATIC_TITLELINE:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_TitleLine"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.9f;
		break;

	case UISTATIC_MENUDECO_L:
		if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Common_Deco_UpperMenuLeft"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.7f;
		break;

	case UISTATIC_MENUDECO_R:
		if (FAILED(__super::Add_Component(LEVEL_LOBBY, TEXT("Prototype_Component_Texture_Common_Deco_UpperMenRight"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		m_fAlpha = 0.7f;
		break;

	default:
		break;
	}
	
	return S_OK;
}

HRESULT CUI_Basic::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Basic::Bind_ShaderResources()
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

void CUI_Basic::Tick_FadeObject(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_Basic::LateTick_FadeObject(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

void CUI_Basic::Ready_Nickname()
{
	m_iMaxNick = 5;

	m_RandomNickname.reserve(m_iMaxNick);
	
	m_RandomNickname.push_back(TEXT("�ʳ׼�����"));
	m_RandomNickname.push_back(TEXT("�����"));
	m_RandomNickname.push_back(TEXT("���̿�����"));
	m_RandomNickname.push_back(TEXT("�������ϰ������"));
	m_RandomNickname.push_back(TEXT("�׺��߸������"));
}

void CUI_Basic::Set_RandomNickname()
{
	// �ֻ����� ����ϴ� �Լ�
	_int iRandom;

	do {
		iRandom = GI->RandomInt(0, m_iMaxNick - 1);

	} while (m_iRandomNum == iRandom);
	// ���� ���� ���ڰ� ������ �������´�.

	// Text�� �������ְ� �����ѹ��� �����Ѵ�.
	CUI_Manager::GetInstance()->Set_RandomNick(m_RandomNickname[iRandom]);
	m_iRandomNum = iRandom;
}

CUI_Basic* CUI_Basic::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, UI_BASIC eType)
{
	CUI_Basic* pInstance = new CUI_Basic(pDevice, pContext, strObjectTag, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Basic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Basic::Clone(void* pArg)
{
	CUI_Basic* pInstance = new CUI_Basic(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Basic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Basic::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
