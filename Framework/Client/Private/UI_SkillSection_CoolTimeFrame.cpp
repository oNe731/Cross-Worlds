#include "stdafx.h"
#include "UI_SkillSection_CoolTimeFrame.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "SKill_Manager.h"
#include "Skill.h"
#include "Game_Manager.h"
#include "Player.h"

CUI_SkillSection_CoolTimeFrame::CUI_SkillSection_CoolTimeFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	UI_COOLFRAME_TYPE eType)
	: CUI(pDevice, pContext, L"UI_SkillSection_CoolTimeFrame")
	, m_eFrameType(eType)
{
}

CUI_SkillSection_CoolTimeFrame::CUI_SkillSection_CoolTimeFrame(const CUI_SkillSection_CoolTimeFrame& rhs)
	: CUI(rhs)
	, m_eFrameType(rhs.m_eFrameType)
{
}

void CUI_SkillSection_CoolTimeFrame::Set_CharacterType(CHARACTER_TYPE eType)
{
	m_eCurPlayerType = eType;
	SetUp_FrameColor();
}

void CUI_SkillSection_CoolTimeFrame::Use_Skill()
{
	m_bUsable = false;
	m_fCurGauge = m_pSkill->Get_CurrCoolTime();
}

HRESULT CUI_SkillSection_CoolTimeFrame::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SkillSection_CoolTimeFrame::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(Ready_Skill()))
		return E_FAIL;

	m_bActive = true; 
	m_fCurGauge = 0.f;

	m_bHide = false;
	m_bHideFinish = false;
	m_vOriginPosition = _float2(m_tInfo.fX, m_tInfo.fY);

	return S_OK;
}

void CUI_SkillSection_CoolTimeFrame::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		Movement_BasedOnHiding(fTimeDelta);

		if (false == m_bUsable)
		{
			m_iPass = 19;

//			m_fCurGauge += fTimeDelta;
			m_fCurGauge = m_pSkill->Get_CurrCoolTime();

			if (m_fMaxGauge <= m_fCurGauge)
			{
				m_bUsable = true;
				m_fCurGauge = m_fMaxGauge;
			}
		}
		else
		{
			m_iPass = 1;
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_SkillSection_CoolTimeFrame::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

	}
}

HRESULT CUI_SkillSection_CoolTimeFrame::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CUI_SkillSection_CoolTimeFrame::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Elemental_Frame"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Elemental_Frame_Mask"),
		TEXT("Com_MaskTexture"), (CComponent**)&m_pMaskTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_SkillSection_CoolTimeFrame::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_SkillSection_CoolTimeFrame::Ready_Skill()
{
	// CurPlayerType �� �������� Ȯ���ؾ���.
	CSkill* pTemp = nullptr;

	CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
	if (pPlayer == nullptr)
		return E_FAIL;
	CCharacter* pCharacter = pPlayer->Get_Character();
	if (pCharacter == nullptr)
		return E_FAIL;

	CHARACTER_TYPE eCharacterType = pCharacter->Get_CharacterType();
	if (CHARACTER_TYPE::CHARACTER_END == eCharacterType)
		return E_FAIL;

	m_eCurPlayerType = eCharacterType;

	switch (m_eCurPlayerType)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		pTemp = CSkill_Manager::GetInstance()->Get_Skill(CHARACTER_TYPE::SWORD_MAN, SKILL_TYPE::SWORDMAN_BURST_MEGA_SLASH);
		if (nullptr == pTemp)
			return E_FAIL;
		m_pSkill = pTemp;
		break;

	case CHARACTER_TYPE::DESTROYER:
		pTemp = CSkill_Manager::GetInstance()->Get_Skill(CHARACTER_TYPE::DESTROYER, SKILL_TYPE::DESTROYER_BURST_HYPERSTRIKE);
		if (nullptr == pTemp)
			return E_FAIL;
		m_pSkill = pTemp;
		break;

	case CHARACTER_TYPE::ENGINEER:
		pTemp = CSkill_Manager::GetInstance()->Get_Skill(CHARACTER_TYPE::ENGINEER, SKILL_TYPE::ENGINNER_BURST_DESTRUCTION);
		if (nullptr == pTemp)
			return E_FAIL;
		m_pSkill = pTemp;
		break;
	}

	m_fMaxGauge = m_pSkill->Get_CoolTime();
	m_fCurGauge = m_pSkill->Get_CurrCoolTime();

	return S_OK;
}

HRESULT CUI_SkillSection_CoolTimeFrame::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eElementalType))))
		return E_FAIL;

	if (!m_bUsable && m_iPass == 19)
	{
		// ���� ��ų�� ������� ���� ���� -> �������� ������.
		_float fRatio = 1 - ((m_fMaxGauge - m_fCurGauge) / m_fMaxGauge); // ���� 0�̸� �� ��ü�� �׷�����.

		if (FAILED(m_pMaskTextureCom->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture")))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_Ratio", &fRatio, sizeof(_float))))
			return E_FAIL;
	}

	return S_OK;
}

void CUI_SkillSection_CoolTimeFrame::SetUp_FrameColor()
{

	if (UI_COOLFRAME_TYPE::FRAME_END <= m_eFrameType)
		return;

	switch (m_eCurPlayerType)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		switch (m_eFrameType)
		{
		case UI_COOLFRAME_TYPE::FRAME_FIRST:
			m_eElementalType = ELEMENTAL_TYPE::FIRE;
			break;

		case UI_COOLFRAME_TYPE::FRAME_SECOND:
			m_eElementalType = ELEMENTAL_TYPE::WATER;
			break;

		case UI_COOLFRAME_TYPE::FRAME_THIRD:
			m_eElementalType = ELEMENTAL_TYPE::WOOD;
			break;
		}
		break;

	case CHARACTER_TYPE::DESTROYER:
		switch (m_eFrameType)
		{
		case UI_COOLFRAME_TYPE::FRAME_FIRST:
			m_eElementalType = ELEMENTAL_TYPE::FIRE;
			break;

		case UI_COOLFRAME_TYPE::FRAME_SECOND:
			m_eElementalType = ELEMENTAL_TYPE::WATER;
			break;

		case UI_COOLFRAME_TYPE::FRAME_THIRD:
			m_eElementalType = ELEMENTAL_TYPE::WOOD;
			break;
		}
		break;

	case CHARACTER_TYPE::ENGINEER:
		switch (m_eFrameType)
		{
		case UI_COOLFRAME_TYPE::FRAME_FIRST:
			m_eElementalType = ELEMENTAL_TYPE::FIRE;
			break;

		case UI_COOLFRAME_TYPE::FRAME_SECOND:
			m_eElementalType = ELEMENTAL_TYPE::WATER;
			break;

		case UI_COOLFRAME_TYPE::FRAME_THIRD:
			m_eElementalType = ELEMENTAL_TYPE::WOOD;
			break;
		}
		break;
	}
}

void CUI_SkillSection_CoolTimeFrame::Movement_BasedOnHiding(_float fTimeDelta)
{
	if (false == m_bHideFinish)
	{
		if (m_bHide) // �����
		{
			if (CUI_Manager::GetInstance()->Get_MovementComplete_SkillBG())
				m_bHideFinish = true;
			else
				m_tInfo.fX += (CUI_Manager::GetInstance()->Get_DistanceofMovement_SkillBG());
		}
		else // �巯����
		{
			if (CUI_Manager::GetInstance()->Get_MovementComplete_SkillBG())
			{
				if (m_tInfo.fX <= m_vOriginPosition.x)
				{
					m_bHideFinish = true;
					m_tInfo.fX = m_vOriginPosition.x;
				}
				else
				{
					m_tInfo.fX -= fTimeDelta * m_fHideSpeed;
				}
			}
			else
			{
				if (m_tInfo.fX <= m_vOriginPosition.x)
				{
					m_bHideFinish = true;
					m_tInfo.fX = m_vOriginPosition.x;
				}
				else
				{
					m_tInfo.fX -= (CUI_Manager::GetInstance()->Get_DistanceofMovement_SkillBG());
				}
			}
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
	}
}

CUI_SkillSection_CoolTimeFrame* CUI_SkillSection_CoolTimeFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	UI_COOLFRAME_TYPE eType)
{
	CUI_SkillSection_CoolTimeFrame* pInstance = new CUI_SkillSection_CoolTimeFrame(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_SkillSection_CoolTimeFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SkillSection_CoolTimeFrame::Clone(void* pArg)
{
	CUI_SkillSection_CoolTimeFrame* pInstance = new CUI_SkillSection_CoolTimeFrame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_SkillSection_CoolTimeFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SkillSection_CoolTimeFrame::Free()
{
	__super::Free();

	Safe_Release(m_pMaskTextureCom);
	Safe_Release(m_pTextureCom);
}
