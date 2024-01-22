#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CShader;
class CTransform;
class CVIBuffer_Cube;
class CTexture;
END

BEGIN(Client)

class CDecal final : public CGameObject
{
public:
	typedef struct tagDecalDesc
	{	
		// ��Į �ڽ� ũ��
		_float3 fScale    = _float3(5.f, 1.f, 5.f);

		// ���� �ð�
		_float  fLifeTime = 50.f;

		// ���ҽ� �� ����
		_int    iTextureIndexDiffuse = 23;

		_uint   iShaderPass    = 1;
		_float  fAlpha_Discard = 0.f;
		_float3 fBlack_Discard = _float3(-1.f, -1.f, -1.f);

		_float3 fBloomPower = _float3(0.5f, 0.5f, 0.5f);
		_float  fBlurPower  = 0.f;

		// ����
		_float  fColorAdd_01_Alpha = 0.9f;               // 01 ���� ���ϴ� ���� ���� ��
		_float3 fColorAdd_01 = _float3(0.4f, 0.8f, 1.f); // ���� �� 0.9 �̻� ���� ����
		_float3 fColorAdd_02 = _float3(0.0f, 0.2f, 0.4f);// ������ ���� ����

		// ����
		_float fAlphaRemove = 0.f; // ���� ���� ������ ���� ���İ�
		_bool bAlphaCreate  = false;
		_bool bAlphaDelete  = false;
		_float fAlphaSpeed  = 0.f;

		// --------------------------------------------------
		// ���� ���� ���ΰ�?
		// ���� �������� �ȼ��� ���ϰ� �����̴� ��� -> �н��� �ٲܱ�? ��� �ʿ�

	} DECAL_DESC;

protected:
	CDecal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CDecal(const CDecal& rhs);
	virtual ~CDecal() = default;

public:
	virtual HRESULT Initialize_Prototype(const DECAL_DESC* pDecalDesc);
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	const DECAL_DESC& Get_DecalDesc() { return m_tDecalDesc; }
	const _float& Get_AccLifeTimeDesc() { return m_fAccLifeTime; }
	class CTexture* Get_DiffuseTexture() { return m_pDiffuseTextureCom; }
	class CTransform* Get_TransformCom() { return m_pTransformCom; }

public:
	void Restart_Decal();

	void Set_DecalDesc(const DECAL_DESC& tDesc);
	void Set_Owner(CGameObject* pGameObject) { m_pOwnerObject = pGameObject; }
	void Set_DeleteDecal(_bool bDecalDelete) { m_bDecalDelete = bDecalDelete; }
	void Set_OffsetPosition(Vec4 vPos) { m_vOffsetPos = vPos; }
	void Set_LifeTime(_float fLifeTime) { m_tDecalDesc.fLifeTime = fLifeTime; };
	void Set_AlphaSpeed(_float fSpeed) { m_tDecalDesc.fAlphaSpeed = fSpeed; };
	void Start_AlphaDeleate();

private:
	void Tick_Alpha(_float fTimeDelta);

private:
	_bool      m_isCloned = { false };
	DECAL_DESC m_tDecalDesc;

private:
	_bool m_bDecalDelete = true;
	_bool  m_bDecalDie = false;

	_float m_fAccLifeTime     = 0.f;
	_bool  m_bAlphaCreateSucc = false;

	Vec4 m_vOffsetPos;

private:
	class CGameObject* m_pOwnerObject = nullptr;

private:
	class CRenderer*  m_pRendererCom  = nullptr;
	class CShader*    m_pShaderCom    = nullptr;
	class CTransform*     m_pTransformCom = nullptr;
	class CVIBuffer_Cube* m_pVIBufferCom = nullptr;
	class CTexture* m_pDiffuseTextureCom = nullptr;

private:
	HRESULT Bind_ShaderResource();

protected:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_Components() override;

public:
	static CDecal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag, const DECAL_DESC* pDecalDesc);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};

END