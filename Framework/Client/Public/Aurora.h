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
class CVIBuffer_Rect;
END

BEGIN(Client)
// scale 1.320, 
class CAurora final : public CGameObject
{
private:
	struct VS_AURORA_DESC
	{
		_float fWaveScale = 1.1f; // ���ζ� ���̺� ũ��
		_float fWaveAmplitude = 0.63934f; // ���ζ� ����
		_float fWaveSpeed = 0.05f; // ���ζ� ���̺� ���ǵ�
		_float fPadding = 0.0f;

		Vec3 vScalar = Vec3(2000.00000f, 130.00000f, 100.00000f); // ���ζ� ��ü ũ�� * Scalar
		_float fPadding2 = 0.0f;
		Vec3 vPosition = Vec3(0.0f, 206.94218f, 190.32748f);
		//Vec3 vPosition = Vec3(0.0f, 306.942184f, 490.32748f); // ���ζ� ��ġ
		_float fPadding3 = 0.0f;
	};

	struct PS_AURORA_DESC
	{
		_float fGlowPhaseScale = 100.0f; // Ÿ�� ���� �Ҳ��� ũ��ܰ�
		_float fGlowWaveSpeed = 0.1f; // Ÿ�� ���� �Ҳ��� ���ǵ�
		_float fGlowPhaseSpeed = 0.32f; // Ÿ�� ���� �Ҳ���
		_float fGlowPhaseBias = 0.2f; // ����

		Vec2 vTextureTiling = Vec2(1.0f,1.0f); // �ؽ��� Ÿ�ϸ�
		Vec2 vTextureSpeed = Vec2(0.001f,0.0f); // �ؽ��� ���ǵ� 
	};

private:
	enum LEVEL_AURORA { WINTER_AURORA, WITCH_AURORA, AURORA_END };

private:
	explicit CAurora(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CAurora(const CAurora& rhs);
	virtual ~CAurora() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT Ready_Components();

private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CVIBuffer_Rect* m_pVIBufferCom = nullptr;
	CModel* m_pModelCom = nullptr;
	CTexture* m_pTextureCom[LEVEL_AURORA::AURORA_END] = {};

private:
	VS_AURORA_DESC m_VSAuroraDesc = {};
	PS_AURORA_DESC m_PSAuroraDesc = {};

	_float m_fTime = 0.0f;
public:
	static CAurora* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

