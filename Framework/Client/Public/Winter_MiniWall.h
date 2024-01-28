#include "MiniGameMap.h"

BEGIN(Client)

class CWinter_MiniWall final : public CMiniGameMap
{
private:
	explicit CWinter_MiniWall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CWinter_MiniWall(const CWinter_MiniWall& rhs);
	virtual ~CWinter_MiniWall() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

public:
	virtual HRESULT Render_Instance(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices);
	virtual HRESULT Render_Instance_Shadow(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices);

public:
	virtual HRESULT Ready_Components() override;

public:
	static CWinter_MiniWall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END