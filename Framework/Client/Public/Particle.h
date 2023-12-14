#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CPipeLine;
class CTransform;
class CRigidBody;
class CVIBuffer_Particle;
END

BEGIN(Client)

class CParticle final : public CGameObject
{
public:
	typedef struct tagParticleDesc
	{	
		// �ݺ� ����
		_bool bLoop = true;

		// ��ƼŬ ����
		_uint iNumEffectCount = 50; 

		// ���� ����
		_float3 fRange = _float3(0.f, 0.f, 0.f);

		// ũ��
		_bool   bSameRate = true; // ������
		_float2 fScale    = _float2(1.f, 1.f);

		// ���ӽð�
		_float2	fLifeTime = _float2(1.f, 5.f);

		// �ӵ�
		_float2 fSpeed = _float2(5.f, 5.f);

		// ������
		_float3 vVelocityMin = _float3(-1.0f, -1.0f, -1.0f);
		_float3 vVelocityMax = _float3(1.0f, 1.0f, 1.0f);

		// �ڽ� ����
		_bool   bUseBox = false;
		_float3 fBoxMin;
		_float3 fBoxMax;

		// ����
		_bool   bRandomColor  = false;
		_float4	vDiffuseColor = _float4(0.f, 0.f, 0.f, 0.f);

		// �ؽ�ó
		_bool bAnimation        = true;
		_bool bAnimationLoop    = true;
		_bool bRandomStartIndex = true;
		_float2 fUVIndex        = _float2(0.f, 0.f);
		_float2 fUVMaxCount     = _float2(1.f, 1.f);
		_float2 fAnimationSpeed = _float2(1.f, 1.f);

		// �ؽ�ó �̸�
		wstring strDiffuseTetextureName = L"";
		wstring strAlphaTexturName      = L"";
		wstring strDiffuseTetexturePath = L"";
		wstring strAlphaTexturPath      = L"";


		// ��Ÿ

		/*
		// Start Delay / ���� ���� ���� �ð�

		_float fDestAlphaSpeed = 0.f;
		_float fBlurPower = 0.01f;

		// RigidBody.
		_bool		bRigidActive    = false;
		_bool		bRandomForceDir = false;
		
		_bool		bRandomFriction = false;
		_float		fFrictionScale = 20.f;
		_float3		vForceDir = { 0.f, 1.f, 0.f };
		_float		fForce = 1.f;
		*/
	} PARTICLE_DESC;

protected:
	CParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CParticle(const CParticle& rhs);
	virtual ~CParticle() = default;

public:
	virtual HRESULT Initialize_Prototype(const PARTICLE_DESC* pParticleDesc, const wstring& strParticleFilePath);
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	const PARTICLE_DESC& Get_ParticleDesc() { return m_tParticleDesc; }
	void Set_ParticleDesc(const PARTICLE_DESC& tDesc);
 
	class CTransform* Get_TransformCom() { return m_pTransformCom; }

private:
	PARTICLE_DESC m_tParticleDesc;

private:
	class CRenderer* m_pRendererCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;

	class CVIBuffer_Particle* m_pVIBufferCom = nullptr;
	class CShader* m_pShaderCom = nullptr;

	class CTexture* m_pDiffuseTextureCom = nullptr;
	class CTexture* m_pAlphaTextureCom = nullptr;

private:
	HRESULT Bind_ShaderResource();

protected:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Components() override;
	void Load_ParticleData(const wstring& strFileName);
	void* Get_ParticleBufferInfo();
	void Set_Texture_Diffuse();
	void Set_Texture_Alpha();

public:
	static CParticle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, 
		const wstring& strObjectTag, const PARTICLE_DESC* pParticleDesc = nullptr, const wstring& strParticleFilePath = {});
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END


