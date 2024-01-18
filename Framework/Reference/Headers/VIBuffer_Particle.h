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

		// ��ƼŬ ����
		_uint* pNumEffectMaxCount = nullptr;

		// ���� ����
		_float3* pRange         = nullptr;
		_float2* pRangeDistance = nullptr;

#pragma region ũ��
		_bool*   pScaleSameRate = nullptr;
		_float2* pScaleStart    = nullptr;

		_bool* pScaleChange             = nullptr;
		_float2* pScaleChangeStartDelay = nullptr;

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

		_bool* pVelocityChange             = nullptr;
		_float2* pVelocityChangeStartDelay = nullptr;

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
		_float2* pRotationChangeStartDelay = nullptr;

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
		_float2* pStartAlpha = nullptr;

		_bool* pFadeCreate  = nullptr;
		_bool* pFadeDelete  = nullptr;
		_float2* pFadeSpeed = nullptr;

		_bool* pFadeChange = nullptr;
		_bool* pFadeIn     = nullptr;
		_float2* pFadeChangeStartDelay = nullptr;
#pragma endregion

#pragma region ����
		_bool* pColorRandom = nullptr;
		_float4* pColorS = nullptr;

		_bool* pColorChange = nullptr;

		_bool* pColorChangeRandom = nullptr;
		_float2* pColorChangeRandomTime = nullptr;

		_bool* pColorLoop   = nullptr;
		_float2* pColorChangeStartDelay = nullptr;

		_float2* pColorChangeStartM = nullptr;
		_float4* pColorM = nullptr;

		_float2* pColorChangeStartF = nullptr;
		_float4* pColorF = nullptr;

		_float2* pColorDuration = nullptr;
#pragma endregion

#pragma region ��
		_bool*   pBloomPowerRandom = nullptr;
		_float4* pBloomPower = nullptr;
		_bool*   pBlurPowerRandom = nullptr;
		_float*  pBlurPower = nullptr;
#pragma endregion

#pragma region ������ٵ�
		_bool* pRigidbody = nullptr;
		_bool* pGravity  = nullptr;
		_bool* pStopZero = nullptr;
		_bool* pStopStartY = nullptr;

		Vec4* pMaxVelocity = nullptr;
		_float* pMass      = nullptr;
		_float* pFricCoeff = nullptr;
#pragma endregion

	} PARTICLE_BUFFER_DESC;

public:
	typedef struct tagParticleDesc
	{
		_bool bIsDie = false;

		// ũ��
		_float fScaleChangeStartTime;
		_float fScaleChangeStartDelay;
		_float fScaleTimeAccs;
		_float fScaleChange;
		_bool  bScaleAdd;
		_float fScaleSpeed;

		// ��ġ
		_float  fVelocityChangeStartTime;
		_float  fVelocityChangeStartDelay;
		_float  fVelocityRanTimeAccs;
		_float  fVelocityRanChange;
		_float  fVelocitySpeeds;
		_float4 vVelocity;
		_uint   iVelocityCountCur;
		_float  fVeloityTimeAccs;
		_float3* pVelocity       = nullptr;
		_float*  pVelocityChange = nullptr;

		// ȸ��
		_float  fRotationChangeStartTime;
		_float  fRotationChangeStartDelay;
		_float  fRotationTime;
		_float  fRotationChangeTime;
		_float  fRotationSpeed;

		// ���� �ð�
		_float fTimeAccs;       
		_float fLifeTimes;      

		// �ִϸ��̼�
		_bool  bAmimationFinish = false;
		_float fAccIndex;
		_float fAnimationSpeed; 

		// ����
		_bool bFadeCreateSucc;
		_float  fAlphaChangeStartTime;
		_float  fAlphaChangeStartDelay;
		_float  fAlphaSpeed;

		// ����
		_float  fColorChangeStartTime;
		_float  fColorChangeStartDelay;

		LERP_VEC3_DESC LerpInfo;
		_uint   iColorIndex;
		_float  fColorChangeStartM;
		_float  fColorChangeStartF;
		_float  fColorChangeEndTime;

		_float  fColorAccs;
		_float  fColorChangeTime;
		_float3 fNextColor;

		_float fViewZ;

	} PARTICLE_INFO_DESC;

	typedef struct tagParticleShaderDesc
	{
		_float2 fUVIndex;  //8
		_float2 fMaxCount; //8

		_float3 fColor; //12
		_float  fAlpha; //4

		_float3 fAxis;  //12
		_float  fAngle; //4

		_float3 fBloomPower; //12
		_float  fBlurPower;  //4

	} PARTICLE_SHADER_DESC;

	typedef struct tagParticleRigidbodyDesc
	{
		Vec4 vForce    = {}; // ũ��, ����
		Vec4 vAccel    = {}; // ���ӵ�
		Vec4 vVelocity = {}; // �ӵ�(ũ��:�ӷ�,����)

		Vec4 vForceA    = {}; // ũ��, ����
		Vec4 vAccelA    = {}; // �߰� ���ӵ�
		Vec4 vVelocityA = {}; // �ӵ�(ũ��:�ӷ�,����)

	} PARTICLE_RIGIDBODY_DESC;

private:
	CVIBuffer_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Particle(const CVIBuffer_Particle& rhs);
	virtual ~CVIBuffer_Particle() = default;

public:
	vector<PARTICLE_SHADER_DESC>& Get_ParticleShaderInfo() { return m_vecParticleShaderDesc; }
	_bool Get_Finished() { return m_bFinished; }

public:
	void Restart_ParticleBufferDesc(_uint iCount);
	void Sort_Z(_uint iCount);
	void Add_Velocity(Vec4 _vMinVelocity, Vec4 _vMaxVelocity);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	void Tick(_float fTimeDelta);
	virtual HRESULT Render(_uint iCount);

private:
	ID3D11Buffer* m_pVBInstance = { nullptr };
	VTXINSTANCE* m_pVertices = { nullptr };

	class CPipeLine* m_pPipeLine = { nullptr };

private:
	_bool m_bFinished = { false };

	_uint m_iStrideInstance = { 0 };
	_uint m_iNumInstance    = { 0 }; // ��ƼŬ ����
	_uint m_iMaxCount = 1000;

	PARTICLE_BUFFER_DESC m_tParticleDesc;
	vector<PARTICLE_INFO_DESC>   m_vecParticleInfoDesc;
	vector<PARTICLE_SHADER_DESC> m_vecParticleShaderDesc;
	vector<PARTICLE_RIGIDBODY_DESC> m_vecParticleRigidbodyDesc;

private:
	_float4 Get_NewPosition_Particle();
	_float4 Get_NewVelocity_Particle();
	void Tick_Rigidbody(_float fTimeDelta, _uint iParticleID);

public:
	static CVIBuffer_Particle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free();
};

END