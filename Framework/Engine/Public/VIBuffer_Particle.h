#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Particle final : public CVIBuffer
{
public:
	typedef struct tagParticleBufferDesc
	{
		// �ݺ� ����
		_bool bLoop;

		// ���� ����
		_float3 fRange;

		// ũ��
		_bool bSameRate;
		_float2 fScale;

		// ���ӽð�
		_float2	fLifeTime;

		// �ӵ�
		_float2 fSpeed;

		// ������
		_float3 vVelocityMin;
		_float3 vVelocityMax;

		// �ڽ� ����
		_bool bUseBox;
		_float3 fBoxMin;
		_float3 fBoxMax;

		// ����
		_bool bRandomColor;
		_float4	vDiffuseColor;

		// �ؽ�ó
		_bool bAnimation;
		_bool bAnimationLoop;
		_bool bRandomStartIndex;
		_float2 fUVIndex;
		_float2 fUVMaxCount;
		_float2 fAnimationSpeed;

	} PARTICLE_BUFFER_DESC;

public:
	typedef struct tagParticleDesc
	{
		_float fTimeAccs;       
		_float fLifeTimes;      

		_float4 vVelocity;
		_float fSpeeds;         

		_float fAccIndex;
		_float fAnimationSpeed; 

	} PARTICLE_INFO_DESC;

	typedef struct tagParticleShaderDesc
	{
		_float2 fUVIndex;  //8
		_float2 fMaxCount; //8

		_float4 vColor; //16

	} PARTICLE_SHADER_DESC;

private:
	CVIBuffer_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Particle(const CVIBuffer_Particle& rhs);
	virtual ~CVIBuffer_Particle() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	void Tick(_float fTimeDelta);
	virtual HRESULT Render(_uint iCount);

public:
	void Set_ParticleBufferDesc(const PARTICLE_BUFFER_DESC& tDesc);
	vector<PARTICLE_SHADER_DESC>& Get_ParticleShaderInfo() { return m_vecParticleShaderDesc; }

	_bool Get_Finished() { return m_bFinished; }

protected:
	_uint m_iStrideInstance = { 0 };
	_uint m_iNumInstance    = { 0 }; // ��ƼŬ ����

	ID3D11Buffer* m_pVBInstance = { nullptr };
	VTXINSTANCE* m_pVertices = { nullptr };

protected:
	PARTICLE_BUFFER_DESC m_tParticleDesc;

	_uint m_iMaxCount = 1000;
	vector<PARTICLE_INFO_DESC>   m_vecParticleInfoDesc;
	vector<PARTICLE_SHADER_DESC> m_vecParticleShaderDesc;

	_bool m_bFinished = { false };

public:
	static CVIBuffer_Particle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free();
};

END