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
		_bool bLoop = false;

		// ��ƼŬ ����
		_uint iNumEffectCount = 1; 

		// ���� ����
		_float3 fRange;

		// ũ��
		_bool bSameRate = true; // ������
		_float2 fScale;

		// ���ӽð�
		_float2	fLifeTime;

		// �ӵ�
		_float2 fSpeed;

		// ������
		_float3 vVelocityMin;
		_float3 vVelocityMax;

		// �ڽ� ����
		_bool bUseBox = false;
		_float3 fBoxMin;
		_float3 fBoxMax;

		// ����
		_bool bRandomColor = false;
		_float4	vDiffuseColor = _float4(0.f, 0.f, 0.f, 0.f);

		// �ؽ�ó
		_bool bAnimation = false;
		_bool bAnimationLoop = false;
		_bool bRandomStartIndex = true;
		_float fDiffuseTextureIndex;
		_float2 fAnimationSpeed;

		wstring strDiffuseTetextureName = L"";
		wstring strAlphaTexturName      = L"";

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
	CParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const wstring& strEffectName);
	CParticle(const CParticle& rhs);
	virtual ~CParticle() = default;

public:
	virtual HRESULT Initialize_Prototype(const PARTICLE_DESC& tEffectDesc);
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	const PARTICLE_DESC& Get_ParticleDesc() { return m_tParticleDesc; }
	void Set_ParticleDesc(const PARTICLE_DESC& tDesc);

	const wstring& Get_EffectPrototypeName() { return m_strPrototypeEffectTag; }
	void Set_EffectPrototypeName(const wstring& strPrototypeEffectTag) { m_strPrototypeEffectTag = strPrototypeEffectTag; }
 
	class CTransform* Get_TransformCom() { return m_pTransformCom; }

private:
	wstring m_strPrototypeEffectTag; // ����Ʈ ���� �̸� �±�

	PARTICLE_DESC m_tParticleDesc;

private:
	class CRenderer* m_pRendererCom = nullptr;
	class CTransform* m_pTransformCom = nullptr;

	class CVIBuffer_Particle* m_pVIBufferCom = nullptr;
	class CShader* m_pShaderCom = nullptr;

	class CTexture* m_pDiffuseTextureCom = nullptr;
	class CTexture* m_pAlphaTextureCom = nullptr;

	LERP_FLOAT_DESC m_Lerp;

private:
	HRESULT Bind_ShaderResource();

protected:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Components() override;

public:
	static CParticle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, 
		const wstring& strObjectTag, const wstring& strPrototypeEffectTag, const PARTICLE_DESC& ParticleDesc);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END


