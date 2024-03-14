#pragma once

#include "Client_Defines.h"
#include "Water.h"


BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CNavigation;
class CVIBuffer_Terrain;
class CModel;
END

BEGIN(Client)

class CEvermoreWater final : public CWater
{
private:
	enum TEX_TYPE { TEX_DIFFUSE, TEX_NOISE1, TEX_NOISE2, TEX_NORMAL, TEX_END };


private:
	explicit CEvermoreWater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CEvermoreWater(const CWater& rhs);
	virtual ~CEvermoreWater() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private: /* �ش� ��ü�� ����ؾ��� ������Ʈ���� �����ϳ�. */ 
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom[TEX_TYPE::TEX_END] = {};
	CModel* m_pModelCom = nullptr;
	CVIBuffer_Terrain* m_pVIBufferCom = { nullptr };

	

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CEvermoreWater* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override; /* �纻��ü�� �����Ҷ� ���������ͷκ��� �����ؿ� �����Ϳܿ� �� �߰��� ����� �����Ͱ� �ִٶ�� �޾ƿ��ڴ�. */
	virtual void Free() override;
};

END

