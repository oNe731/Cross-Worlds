#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Particle final : public CVIBuffer
{
public:
	typedef struct tagParticleBufferDesc
	{
		// ��ƼŬ ����
        _uint iNumCount;

		_bool bLoop = false;

		// ũ��
		_bool bSameRate = true;
		_float2 fScale;

		// ���� ����
		_float3 fRange;

		// ��(�̵�)
		_float3 vVelocityMin;
		_float3 vVelocityMax;

		// �ӵ�
		_float2 fSpeed;

		// �ð�
		_float2	fLifeTime;

		// �ڽ� ����
		_float3 fBoxMin;
		_float3 fBoxMax;

	} PARTICLE_BUFFER_DESC;

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
	_bool Get_Finished() { return m_bFinished; };


protected:
	_uint m_iStrideInstance = { 0 };
	_uint m_iNumInstance    = { 0 }; // ��ƼŬ ����

	ID3D11Buffer* m_pVBInstance = { nullptr };
	VTXINSTANCE* m_pVertices = { nullptr };

protected:
	PARTICLE_BUFFER_DESC m_tParticleDesc;

	_bool m_bFinished = { false };

	_float* m_pTimeAccs = { nullptr };
	_float* m_pLifeTimes = { nullptr };

	_float4* m_vVelocity = { nullptr };
	_float* m_pSpeeds = { nullptr };

public:
	static CVIBuffer_Particle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free();
};

END