#include "stdafx.h"
#include "Particle.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "Utils.h"
#include "VIBuffer_Particle.h"
#include "filesystem"
#include "FileUtils.h"
#include "Utils.h"

CParticle::CParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_PARTICLE)
	, m_isCloned(false)
{
}

CParticle::CParticle(const CParticle& rhs)
	: CGameObject(rhs)
	, m_isCloned(true)
	, m_tParticleDesc(rhs.m_tParticleDesc)
	, m_ViewMatrix(rhs.m_ViewMatrix)
	, m_ProjMatrix(rhs.m_ProjMatrix)
{
}

void CParticle::Set_ParticleDesc(const PARTICLE_DESC& tDesc)
{
	m_tParticleDesc = tDesc;

	// �ؽ�ó ����
	Set_Texture_Diffuse();
	Set_Texture_Alpha();

	// ���� �����
	if (m_pVIBufferCom != nullptr)
		m_pVIBufferCom->Restart_ParticleBufferDesc(m_tParticleDesc.iNumEffectCount);
}

void CParticle::Set_Position_Perspective(_float3 fPosition)
{
	m_tParticleDesc.eParticleType = TYPE_PERSPECTIVE;

	if(m_pTransformCom != nullptr)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPosition.x, fPosition.y, fPosition.z, 1.f));
}

void CParticle::Set_Position_Orthographic(_float2 fPosition)
{
	m_tParticleDesc.eParticleType = TYPE_ORTHOGRAPHIC;

	if (m_pTransformCom != nullptr)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(fPosition.x - g_iWinSizeX * 0.5f, -fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
}

HRESULT CParticle::Initialize_Prototype(const PARTICLE_DESC* pParticleDesc, const wstring& strParticleFilePath)
{
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	if (pParticleDesc != nullptr)
	{
		m_tParticleDesc = *pParticleDesc;
		if (m_tParticleDesc.iVelocityUse > 0)
		{
			if(m_tParticleDesc.pVelocityMin == nullptr)
				m_tParticleDesc.pVelocityMin = new _float3[m_tParticleDesc.iVelocityUse];

			if (m_tParticleDesc.pVelocityMax == nullptr)
				m_tParticleDesc.pVelocityMax = new _float3[m_tParticleDesc.iVelocityUse];

			if (m_tParticleDesc.pVelocityTime == nullptr)
				m_tParticleDesc.pVelocityTime = new _float2[m_tParticleDesc.iVelocityUse];
		}
	}
	else
		Load_ParticleData(strParticleFilePath);

 	return S_OK;
}

HRESULT CParticle::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (pArg != nullptr)
	{
		_vector vPosition = *static_cast<_vector*>(pArg);

		if (m_tParticleDesc.eParticleType == TYPE_PERSPECTIVE)
			Set_Position_Perspective(_float3(XMVectorGetX(vPosition), XMVectorGetY(vPosition), XMVectorGetZ(vPosition)));
		else if (m_tParticleDesc.eParticleType == TYPE_ORTHOGRAPHIC)
			Set_Position_Orthographic(_float2(XMVectorGetX(vPosition), XMVectorGetY(vPosition)));
	}

	return S_OK;
}

void CParticle::Tick(_float fTimeDelta)
{
	m_pVIBufferCom->Tick(fTimeDelta);

#ifdef _DEBUG
	if (GI->Get_CurrentLevel() != LEVEL_TOOL && m_pVIBufferCom->Get_Finished())
		Set_Dead(true);
#else // ������
	if (m_pVIBufferCom->Get_Finished())
		Set_Dead(true);
#endif
}

void CParticle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if(m_tParticleDesc.bParticleSortZ)
		m_pVIBufferCom->Sort_Z(m_tParticleDesc.iNumEffectCount);

	if(m_tParticleDesc.eParticleType == TYPE_PERSPECTIVE)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this);
	else if(m_tParticleDesc.eParticleType == TYPE_ORTHOGRAPHIC)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UIEFFECT, this);
}

HRESULT CParticle::Render()
{
	if (FAILED(Bind_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(m_tParticleDesc.iShaderPass);
	m_pVIBufferCom->Render(m_tParticleDesc.iNumEffectCount);

	return S_OK;
}

HRESULT CParticle::Bind_ShaderResource()
{
	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;

	if (m_tParticleDesc.eParticleType == TYPE_PERSPECTIVE)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
			return E_FAIL;
	}
	else if (m_tParticleDesc.eParticleType == TYPE_ORTHOGRAPHIC)
	{
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	// ����Ʈ ����
	if (FAILED(m_pShaderCom->Bind_RawValue("g_EffectDesc", m_pVIBufferCom->Get_ParticleShaderInfo().data(), sizeof(CVIBuffer_Particle::PARTICLE_SHADER_DESC) * m_pVIBufferCom->Get_ParticleShaderInfo().size())))
		return E_FAIL;

	// �ؽ�ó
	if (m_pDiffuseTextureCom != nullptr)
	{
		if (FAILED(m_pDiffuseTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_tParticleDesc.iTextureIndexDiffuse)))
			return E_FAIL;
	}
	if (m_pAlphaTextureCom != nullptr)
	{
		if (FAILED(m_pAlphaTextureCom->Bind_ShaderResource(m_pShaderCom, "g_AlphaTexture", m_tParticleDesc.iTextureIndexAlpha)))
			return E_FAIL;
	}

	if (FAILED(m_pShaderCom->Bind_RawValue("g_bBillboard", &m_tParticleDesc.bBillboard, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha_Discard", &m_tParticleDesc.fAlpha_Discard, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fBlack_Discard", &m_tParticleDesc.fBlack_Discard, sizeof(_float3))))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Point_Instance"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Texture */
	Set_Texture_Diffuse();
	Set_Texture_Alpha();

	/* For.Com_VIBuffer */
	CVIBuffer_Particle::PARTICLE_BUFFER_DESC pBufferInfo = *static_cast<CVIBuffer_Particle::PARTICLE_BUFFER_DESC*>(Get_ParticleBufferInfo());
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom, &pBufferInfo)))
			return E_FAIL;

	return S_OK;
}

void CParticle::Load_ParticleData(const wstring& strFileName)
{
	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strFileName, FileMode::Read);

#pragma region 2023-12-16-ver.1

	// ��ƼŬ Ÿ��
	_uint iParticleType = 0;
	File->Read<_uint>(iParticleType);
	m_tParticleDesc.eParticleType = (CParticle::PARTICLEPROJTYPE)iParticleType;

	// ���� ����
	File->Read<_bool>(m_tParticleDesc.bParticleSortZ);

	// �ݺ� ����
	File->Read<_bool>(m_tParticleDesc.bParticleLoop);

	// ��ƼŬ ����
	File->Read<_uint>(m_tParticleDesc.iNumEffectMaxCount);
	File->Read<_uint>(m_tParticleDesc.iNumEffectCount);

	// ���� ����
	File->Read<_float3>(m_tParticleDesc.fRange);
	File->Read<_float2>(m_tParticleDesc.fRangeDistance);

#pragma region ũ��
	File->Read<_bool>(m_tParticleDesc.bScaleSameRate);
	File->Read<_float2>(m_tParticleDesc.fScaleStart);

	File->Read<_bool>(m_tParticleDesc.bScaleChange);
	File->Read<_float2>(m_tParticleDesc.fScaleChangeStartDelay);

	File->Read<_bool>(m_tParticleDesc.bScaleChangeRandom);
	File->Read<_float2>(m_tParticleDesc.fScaleChangeTime);

	File->Read<_bool>(m_tParticleDesc.bScaleAdd);
	File->Read<_bool>(m_tParticleDesc.bScaleLoop);
	File->Read<_bool>(m_tParticleDesc.bScaleLoopStart);

	File->Read<_float2>(m_tParticleDesc.fScaleMin);
	File->Read<_float2>(m_tParticleDesc.fScaleMax);
	File->Read<_float2>(m_tParticleDesc.fScaleSpeed);
#pragma endregion

#pragma region �̵�
	File->Read<_float2>(m_tParticleDesc.fVelocitySpeed);

	File->Read<_float3>(m_tParticleDesc.vVelocityMinStart);
	File->Read<_float3>(m_tParticleDesc.vVelocityMaxStart);

	File->Read<_bool>(m_tParticleDesc.bVelocityChange);
	File->Read<_float2>(m_tParticleDesc.fVelocityChangeStartDelay);

	File->Read<_bool>(m_tParticleDesc.bVelocityChangeRandom);
	File->Read<_float2>(m_tParticleDesc.fVelocityChangeTime);

	File->Read<_bool>(m_tParticleDesc.bVelocityLoop);
	File->Read<_uint>(m_tParticleDesc.iVelocityCountCur);
	File->Read<_uint>(m_tParticleDesc.iVelocityCountMax);

	File->Read<_uint>(m_tParticleDesc.iVelocityUse);
	if (m_tParticleDesc.iVelocityUse > 0)
	{
		m_tParticleDesc.pVelocityMin = new _float3[m_tParticleDesc.iVelocityUse];
		m_tParticleDesc.pVelocityMax = new _float3[m_tParticleDesc.iVelocityUse];
		m_tParticleDesc.pVelocityTime = new _float2[m_tParticleDesc.iVelocityUse];
		if (m_tParticleDesc.pVelocityMin != nullptr)
		{
			for (size_t i = 0; i < m_tParticleDesc.iVelocityUse; ++i)
				File->Read<_float3>(m_tParticleDesc.pVelocityMin[i]);
		}

		if (m_tParticleDesc.pVelocityMax != nullptr)
		{
			for (size_t i = 0; i < m_tParticleDesc.iVelocityUse; ++i)
				File->Read<_float3>(m_tParticleDesc.pVelocityMax[i]);
		}

		if (m_tParticleDesc.pVelocityTime != nullptr)
		{
			for (size_t i = 0; i < m_tParticleDesc.iVelocityUse; ++i)
				File->Read<_float2>(m_tParticleDesc.pVelocityTime[i]);
		}
	}
#pragma endregion

#pragma region ȸ��
	File->Read<_bool>(m_tParticleDesc.bBillboard);

	File->Read<_bool>(m_tParticleDesc.bRandomAxis);
	File->Read<_vector>(m_tParticleDesc.vAxis);

	File->Read<_bool>(m_tParticleDesc.bRandomAngle);
	File->Read<_float>(m_tParticleDesc.fAngle);

	File->Read<_bool>(m_tParticleDesc.bRotationChange);
	File->Read<_float2>(m_tParticleDesc.fRotationChangeStartDelay);

	File->Read<_float2>(m_tParticleDesc.fRotationSpeed);

	File->Read<_bool>(m_tParticleDesc.bRotationChangeRandom);
	File->Read<_float2>(m_tParticleDesc.fRotationChangeTime);

	File->Read<_bool>(m_tParticleDesc.bRotationAdd);
#pragma endregion

	// ���� �ð�
	File->Read<_float2>(m_tParticleDesc.fLifeTime);

	// �ڽ� ����
	File->Read<_bool>(m_tParticleDesc.bUseBox);
	File->Read<_float3>(m_tParticleDesc.fBoxMin);
	File->Read<_float3>(m_tParticleDesc.fBoxMax);

#pragma region �ؽ�ó
	// ���ڿ�
	m_tParticleDesc.strDiffuseTetextureName = CUtils::ToWString(File->Read<string>());
	m_tParticleDesc.strDiffuseTetexturePath = CUtils::ToWString(File->Read<string>());
	m_tParticleDesc.strAlphaTexturName = CUtils::ToWString(File->Read<string>());
	m_tParticleDesc.strAlphaTexturPath = CUtils::ToWString(File->Read<string>());

	File->Read<_bool>(m_tParticleDesc.bRandomStartIndex);
	File->Read<_float2>(m_tParticleDesc.fUVIndex);
	File->Read<_float2>(m_tParticleDesc.fUVMaxCount);

	File->Read<_uint>(m_tParticleDesc.iTextureIndexDiffuse);
	File->Read<_uint>(m_tParticleDesc.iTextureIndexAlpha);
#pragma endregion

#pragma region �ִϸ��̼�
	File->Read<_bool>(m_tParticleDesc.bAnimation);
	File->Read<_bool>(m_tParticleDesc.bAnimationLoop);
	File->Read<_float2>(m_tParticleDesc.fAnimationSpeed);
#pragma endregion

#pragma region ����
	File->Read<_float2>(m_tParticleDesc.fStartAlpha);

	File->Read<_bool>(m_tParticleDesc.bFadeCreate);
	File->Read<_bool>(m_tParticleDesc.bFadeDelete);
	File->Read<_float2>(m_tParticleDesc.fFadeSpeed);

	File->Read<_bool>(m_tParticleDesc.bFadeChange);
	File->Read<_bool>(m_tParticleDesc.bFadeIn);
	File->Read<_float2>(m_tParticleDesc.fFadeChangeStartDelay);
#pragma endregion

#pragma region ����
	File->Read<_bool>(m_tParticleDesc.bColorRandom);
	File->Read<_float4>(m_tParticleDesc.vColorS);

	File->Read<_bool>(m_tParticleDesc.bColorChange);

	File->Read<_bool>(m_tParticleDesc.bColorChangeRandom);
	File->Read<_float2>(m_tParticleDesc.fColorChangeRandomTime);

	File->Read<_bool>(m_tParticleDesc.bColorLoop);
	File->Read<_float2>(m_tParticleDesc.fColorChangeStartDelay);

	File->Read<_float2>(m_tParticleDesc.fColorChangeStartM);
	File->Read<_float4>(m_tParticleDesc.fColorM);

	File->Read<_float2>(m_tParticleDesc.fColorChangeStartF);
	File->Read<_float4>(m_tParticleDesc.fColorF);

	File->Read<_float2>(m_tParticleDesc.fColorDuration);
#pragma endregion

#pragma region ��
	File->Read<_bool>(m_tParticleDesc.bBlurColorRandom);
	File->Read<_float4>(m_tParticleDesc.fBlurColor);
	File->Read<_bool>(m_tParticleDesc.bBlurPowerRandom);
	File->Read<_float>(m_tParticleDesc.fBlurPower);
#pragma endregion

#pragma region ��Ÿ ����
	File->Read<_uint>(m_tParticleDesc.iShaderPass);
	File->Read<_float>(m_tParticleDesc.fAlpha_Discard);
	File->Read<_float3>(m_tParticleDesc.fBlack_Discard);
#pragma endregion

	// ���� �߰� �� ���� ���� �ε� �� �߰� �� ���� �� �������ϱ�

#pragma endregion

}

void* CParticle::Get_ParticleBufferInfo()
{
	CVIBuffer_Particle::PARTICLE_BUFFER_DESC tBufferInfo = {};

	tBufferInfo.pParticleLoop = &m_tParticleDesc.bParticleLoop;

	tBufferInfo.pNumEffectMaxCount = &m_tParticleDesc.iNumEffectMaxCount;

	tBufferInfo.pRange         = &m_tParticleDesc.fRange;
	tBufferInfo.pRangeDistance = &m_tParticleDesc.fRangeDistance;

	tBufferInfo.pScaleSameRate = &m_tParticleDesc.bScaleSameRate;
	tBufferInfo.pScaleStart    = &m_tParticleDesc.fScaleStart;

	tBufferInfo.pScaleChange           = &m_tParticleDesc.bScaleChange;
	tBufferInfo.pScaleChangeStartDelay = &m_tParticleDesc.fScaleChangeStartDelay;

	tBufferInfo.pScaleChangeRandom = &m_tParticleDesc.bScaleChangeRandom;
	tBufferInfo.pScaleChangeTime   = &m_tParticleDesc.fScaleChangeTime;

	tBufferInfo.pScaleAdd       = &m_tParticleDesc.bScaleAdd;
	tBufferInfo.pScaleLoop      = &m_tParticleDesc.bScaleLoop;
	tBufferInfo.pScaleLoopStart = &m_tParticleDesc.bScaleLoopStart;

	tBufferInfo.pScaleMin       = &m_tParticleDesc.fScaleMin;
	tBufferInfo.pScaleMax       = &m_tParticleDesc.fScaleMax;
	tBufferInfo.pScaleSpeed     = &m_tParticleDesc.fScaleSpeed;


	tBufferInfo.pVelocitySpeed = &m_tParticleDesc.fVelocitySpeed;

	tBufferInfo.pVelocityMinStart = &m_tParticleDesc.vVelocityMinStart;
	tBufferInfo.pVelocityMaxStart = &m_tParticleDesc.vVelocityMaxStart;

	tBufferInfo.pVelocityChange           = &m_tParticleDesc.bVelocityChange;
	tBufferInfo.pVelocityChangeStartDelay = &m_tParticleDesc.fVelocityChangeStartDelay;

	tBufferInfo.pVelocityChangeRandom = &m_tParticleDesc.bVelocityChangeRandom;
	tBufferInfo.pVelocityChangeTime   = &m_tParticleDesc.fVelocityChangeTime;

	tBufferInfo.pVelocityLoop   = &m_tParticleDesc.bVelocityLoop;
	tBufferInfo.pVelocityCountCur = &m_tParticleDesc.iVelocityCountCur;
	tBufferInfo.pVelocityCountMax = &m_tParticleDesc.iVelocityCountMax;

	tBufferInfo.pVelocityUse = &m_tParticleDesc.iVelocityUse;
	tBufferInfo.pVelocityMin  = m_tParticleDesc.pVelocityMin;
	tBufferInfo.pVelocityMax  = m_tParticleDesc.pVelocityMax;
	tBufferInfo.pVelocityTime = m_tParticleDesc.pVelocityTime;


	tBufferInfo.pBillboard   = &m_tParticleDesc.bBillboard;
	tBufferInfo.pRandomAxis  = &m_tParticleDesc.bRandomAxis;
	tBufferInfo.pAxis        = &m_tParticleDesc.vAxis;
	tBufferInfo.pRandomAngle = &m_tParticleDesc.bRandomAngle;
	tBufferInfo.pAngle       = &m_tParticleDesc.fAngle;

	tBufferInfo.pRotationChange           = &m_tParticleDesc.bRotationChange;
	tBufferInfo.pRotationChangeStartDelay = &m_tParticleDesc.fRotationChangeStartDelay;

	tBufferInfo.pRotationChangeRandom = &m_tParticleDesc.bRotationChangeRandom;
	tBufferInfo.pRotationChangeTime   = &m_tParticleDesc.fRotationChangeTime;
	tBufferInfo.pRotationAdd   = &m_tParticleDesc.bRotationAdd;
	tBufferInfo.pRotationSpeed = &m_tParticleDesc.fRotationSpeed;


	tBufferInfo.pLifeTime = &m_tParticleDesc.fLifeTime;

	tBufferInfo.pUseBox = &m_tParticleDesc.bUseBox;
	tBufferInfo.pBoxMin = &m_tParticleDesc.fBoxMin;
	tBufferInfo.pBoxMax = &m_tParticleDesc.fBoxMax;

	tBufferInfo.pRandomStartIndex = &m_tParticleDesc.bRandomStartIndex;
	tBufferInfo.pUVIndex    = &m_tParticleDesc.fUVIndex;
	tBufferInfo.pUVMaxCount = &m_tParticleDesc.fUVMaxCount;

	tBufferInfo.pAnimation      = &m_tParticleDesc.bAnimation;
	tBufferInfo.pAnimationLoop  = &m_tParticleDesc.bAnimationLoop;
	tBufferInfo.pAnimationSpeed = &m_tParticleDesc.fAnimationSpeed;


	tBufferInfo.pFadeCreate = &m_tParticleDesc.bFadeCreate;
	tBufferInfo.pFadeDelete = &m_tParticleDesc.bFadeDelete;

	tBufferInfo.pFadeChange           = &m_tParticleDesc.bFadeChange;
	tBufferInfo.pFadeChangeStartDelay = &m_tParticleDesc.fFadeChangeStartDelay;
	tBufferInfo.pFadeIn     = &m_tParticleDesc.bFadeIn;
	tBufferInfo.pStartAlpha = &m_tParticleDesc.fStartAlpha;
	tBufferInfo.pFadeSpeed  = &m_tParticleDesc.fFadeSpeed;


	tBufferInfo.pColorRandom = &m_tParticleDesc.bColorRandom;
	tBufferInfo.pColorS      = &m_tParticleDesc.vColorS;

	tBufferInfo.pColorChange = &m_tParticleDesc.bColorChange;

	tBufferInfo.pColorChangeRandom = &m_tParticleDesc.bColorChangeRandom;
	tBufferInfo.pColorChangeRandomTime = &m_tParticleDesc.fColorChangeRandomTime;

	tBufferInfo.pColorLoop   = &m_tParticleDesc.bColorLoop;
	tBufferInfo.pColorChangeStartDelay = &m_tParticleDesc.fColorChangeStartDelay;

	tBufferInfo.pColorChangeStartM = &m_tParticleDesc.fColorChangeStartM;
	tBufferInfo.pColorM            = &m_tParticleDesc.fColorM;

	tBufferInfo.pColorChangeStartF = &m_tParticleDesc.fColorChangeStartF;
	tBufferInfo.pColorF            = &m_tParticleDesc.fColorF;

	tBufferInfo.pColorDuration = &m_tParticleDesc.fColorDuration;


	tBufferInfo.pBlurColorRandom = &m_tParticleDesc.bBlurColorRandom;
	tBufferInfo.pBlurColor       = &m_tParticleDesc.fBlurColor;
	tBufferInfo.pBlurPowerRandom = &m_tParticleDesc.bBlurPowerRandom;
	tBufferInfo.pBlurPower       = &m_tParticleDesc.fBlurPower;


	return &tBufferInfo;
}

void CParticle::Set_Texture_Diffuse()
{
	_int  iBufferSizeName = WideCharToMultiByte(CP_UTF8, 0, m_tParticleDesc.strDiffuseTetextureName.c_str(), -1, nullptr, 0, nullptr, nullptr);
	char* pszFileName     = new char[iBufferSizeName];
	WideCharToMultiByte(CP_UTF8, 0, m_tParticleDesc.strDiffuseTetextureName.c_str(), -1, pszFileName, iBufferSizeName, nullptr, nullptr);
	if (strcmp(pszFileName, "") != 0)
	{
		if (m_pDiffuseTextureCom != nullptr)
			Safe_Release(m_pDiffuseTextureCom);

		m_pDiffuseTextureCom = static_cast<CTexture*>(GI->Clone_Component(LEVEL_STATIC, m_tParticleDesc.strDiffuseTetextureName));
	}
	else
	{
		_int  iBufferSizePath = WideCharToMultiByte(CP_UTF8, 0, m_tParticleDesc.strDiffuseTetexturePath.c_str(), -1, nullptr, 0, nullptr, nullptr);
		char* pszFilePath     = new char[iBufferSizePath];
		WideCharToMultiByte(CP_UTF8, 0, m_tParticleDesc.strDiffuseTetexturePath.c_str(), -1, pszFilePath, iBufferSizePath, nullptr, nullptr);
		if (strcmp(pszFilePath, "") != 0)
		{
			if (m_pDiffuseTextureCom != nullptr)
				Safe_Release(m_pDiffuseTextureCom);

			char szFileName[MAX_PATH] = ""; // �����̸�
			_splitpath_s(pszFilePath, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);
			if (strcmp(szFileName, "") == 0)
				m_pDiffuseTextureCom = CTexture::Create(m_pDevice, m_pContext, m_tParticleDesc.strDiffuseTetexturePath, 0, true);
			else
				m_pDiffuseTextureCom = CTexture::Create(m_pDevice, m_pContext, m_tParticleDesc.strDiffuseTetexturePath);
		}
		Safe_Delete(pszFilePath);
	}
	Safe_Delete(pszFileName);

	if (m_pDiffuseTextureCom != nullptr && m_tParticleDesc.iTextureIndexDiffuse > m_pDiffuseTextureCom->Get_TextureCount())
		m_tParticleDesc.iTextureIndexDiffuse = m_pDiffuseTextureCom->Get_TextureCount();
}

void CParticle::Set_Texture_Alpha()
{
	_int  iBufferSizeName = WideCharToMultiByte(CP_UTF8, 0, m_tParticleDesc.strAlphaTexturName.c_str(), -1, nullptr, 0, nullptr, nullptr);
	char* pszFileName = new char[iBufferSizeName];
	WideCharToMultiByte(CP_UTF8, 0, m_tParticleDesc.strAlphaTexturName.c_str(), -1, pszFileName, iBufferSizeName, nullptr, nullptr);
	if (strcmp(pszFileName, "") != 0)
	{
		if (m_pAlphaTextureCom != nullptr)
			Safe_Release(m_pAlphaTextureCom);

		m_pAlphaTextureCom = static_cast<CTexture*>(GI->Clone_Component(LEVEL_STATIC, m_tParticleDesc.strAlphaTexturName));
	}
	else
	{
		_int  iBufferSizePath = WideCharToMultiByte(CP_UTF8, 0, m_tParticleDesc.strAlphaTexturPath.c_str(), -1, nullptr, 0, nullptr, nullptr);
		char* pszFilePath = new char[iBufferSizePath];
		WideCharToMultiByte(CP_UTF8, 0, m_tParticleDesc.strAlphaTexturPath.c_str(), -1, pszFilePath, iBufferSizePath, nullptr, nullptr);
		if (strcmp(pszFilePath, "") != 0)
		{
			if (m_pAlphaTextureCom != nullptr)
				Safe_Release(m_pAlphaTextureCom);

			char szFileName[MAX_PATH] = ""; // �����̸�
			_splitpath_s(pszFilePath, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, nullptr, 0);
			if (strcmp(szFileName, "") == 0)
				m_pAlphaTextureCom = CTexture::Create(m_pDevice, m_pContext, m_tParticleDesc.strAlphaTexturPath, 0, true);
			else
				m_pAlphaTextureCom = CTexture::Create(m_pDevice, m_pContext, m_tParticleDesc.strAlphaTexturPath);
		}
		Safe_Delete(pszFilePath);
	}
	Safe_Delete(pszFileName);

	if (m_pAlphaTextureCom != nullptr && m_tParticleDesc.iTextureIndexAlpha > m_pAlphaTextureCom->Get_TextureCount())
		m_tParticleDesc.iTextureIndexAlpha = m_pAlphaTextureCom->Get_TextureCount();
}

CParticle* CParticle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const PARTICLE_DESC* pParticleDesc, const wstring& strParticleFilePath)
{
	CParticle* pInstance = new CParticle(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype(pParticleDesc, strParticleFilePath)))
	{
		MSG_BOX("Failed to Created : CParticle");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CParticle::Clone(void* pArg)
{
	CParticle* pInstance = new CParticle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CParticle");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CParticle::Free()
{
	__super::Free();

	if (!m_isCloned)
	{
		if(m_tParticleDesc.pVelocityMin != nullptr)
			Safe_Delete_Array(m_tParticleDesc.pVelocityMin);

		if (m_tParticleDesc.pVelocityMax != nullptr)
			Safe_Delete_Array(m_tParticleDesc.pVelocityMax);

		if (m_tParticleDesc.pVelocityTime != nullptr)
			Safe_Delete_Array(m_tParticleDesc.pVelocityTime);
	}

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pDiffuseTextureCom);
	Safe_Release(m_pAlphaTextureCom);
}