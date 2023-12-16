#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CHierarchyNode;
class CModel;
class CShader;
class CTransform;
class CRenderer;
END

BEGIN(Client)

class CPart abstract : public CGameObject
{
protected:
	CPart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObejctTag, _uint iObjectType);
	CPart(const CPart& rhs);

	virtual ~CPart() = default;

public:
	HRESULT	Set_Owner(CGameObject* pOwner);
	class CGameObject* Get_Owner() { return m_pOwner; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) {};
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) {};
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) {};

public:
	virtual void Set_SocketWorld(Matrix matSocketWorld) { memcpy(&m_matSocketWorld, &matSocketWorld, sizeof(Matrix)); } /* ���� ���� �ִϸ��̼� ������ �̷���� �� ȣ�� */

protected: 
	CGameObject* m_pOwner = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	Matrix m_matSocketWorld;

public:
	virtual void Free() override;
};

END