#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Particle final : public CVIBuffer
{
public:
	typedef struct tagParticleBufferDesc
	{
		// �ݺ� ����
		_bool* pParticleLoop = nullptr;

		// ���� ����
		_float3* pRange = nullptr;

#pragma region ũ��
		_bool*   pScaleSameRate = nullptr;
		_float2* pScaleStart    = nullptr;

		_bool* pScaleChange       = nullptr;
		_bool* pScaleChangeRandom = nullptr;
		_float2* pScaleChangeTime = nullptr;

		_bool* pScaleAdd    = nullptr;
		_bool* pScaleLoop   = nullptr;
		_bool* pScaleLoopStart = nullptr;

		_float2* pScaleMin   = nullptr;
		_float2* pScaleMax   = nullptr;
		_float2* pScaleSpeed = nullptr;
#pragma endregion

#pragma region �̵�
		_float2* pVelocitySpeed = nullptr;

		_float3* pVelocityMinStart = nullptr;
		_float3* pVelocityMaxStart = nullptr;

		_bool* pVelocityChange = nullptr;
		_bool* pVelocityChangeRandom = nullptr;
		_float2* pVelocityChangeTime = nullptr;

		_bool* pVelocityLoop     = nullptr;
		_uint* pVelocityCountCur = nullptr;
		_uint* pVelocityCountMax = nullptr;

		_uint* pVelocityUse = nullptr;
		_float3* pVelocityMin  = nullptr;
		_float3* pVelocityMax  = nullptr;
		_float2* pVelocityTime = nullptr;
#pragma endregion

#pragma region ȸ��
		_bool* pBillboard = nullptr;

		_bool* pRandomAxis = nullptr;
		_vector* pAxis = nullptr;

		_bool* pRandomAngle = nullptr;
		_float* pAngle = nullptr;

		_bool* pRotationChange = nullptr;

		_bool* pRotationChangeRandom = nullptr;
		_float2* pRotationChangeTime = nullptr;

		_bool* pRotationAdd = nullptr;

		_float2* pRotationSpeed = nullptr;
#pragma endregion

		// ���� �ð�
		_float2* pLifeTime = nullptr;

		// �ڽ� ����
		_bool* pUseBox = nullptr;
		_float3* pBoxMin = nullptr;
		_float3* pBoxMax = nullptr;

		// �ؽ�ó
		_bool* pRandomStartIndex = nullptr;
		_float2* pUVIndex    = nullptr;
		_float2* pUVMaxCount = nullptr;

		// �ִϸ��̼�
		_bool* pAnimation        = nullptr;
		_bool* pAnimationLoop    = nullptr;
		_float2* pAnimationSpeed = nullptr;

#pragma region ����
		_bool*   pRandomColor = nullptr;
		_float4* pColor       = nullptr;
#pragma endregion

	} PARTICLE_BUFFER_DESC;

public:
	typedef struct tagParticleDesc
	{
		// ũ��
		_float fScaleTimeAccs;
		_float fScaleChange;
		_bool  bScaleAdd;
		_float fScaleSpeed;

		// ��ġ
		_float fVelocityRanTimeAccs;
		_float fVelocityRanChange;
		_float fVelocitySpeeds;
		_float4 vVelocity;
		_uint  iVelocityCountCur;
		_float fVeloityTimeAccs;
		_float3* pVelocity       = nullptr;
		_float*  pVelocityChange = nullptr;

		// ȸ��
		_float fRotationChangeTime;
		_float fRotationSpeed;

		// ���� �ð�
		_float fTimeAccs;       
		_float fLifeTimes;      

		// �ִϸ��̼�
		_bool bAmimationFinish = false;
		_float fAccIndex;
		_float fAnimationSpeed; 

	} PARTICLE_INFO_DESC;

	typedef struct tagParticleShaderDesc
	{
		_float2 fUVIndex;  //8
		_float2 fMaxCount; //8

		_float4 vColor; //16

		_vector vAxis;  //16

		_bool   bBillboard; //4
		_float  fAngle; //4
		_float  fTemp0; //4
		_float  fTemp1; //4

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
	void Restart_ParticleBufferDesc();
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

private:
	_float4 Get_NewPosition_Particle();
	_float4 Get_NewVelocity_Particle();

public:
	static CVIBuffer_Particle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free();
};

END