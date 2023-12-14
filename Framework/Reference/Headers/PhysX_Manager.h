#pragma once

#include "Base.h"
#include "Engine_Defines.h"


BEGIN(Engine)

class CPhysX_Manager final : public CBase, PxSimulationEventCallback
{

public:
	DECLARE_SINGLETON(CPhysX_Manager)

public:
	CPhysX_Manager();
	virtual ~CPhysX_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

#ifdef _DEBUG
public:
	HRESULT Render();
#endif


public:
	PxRigidStatic* Add_Static_Actor(const PHYSX_INIT_DESC& Desc);
	PxRigidDynamic* Add_Dynamic_Actor(const PHYSX_INIT_DESC& Desc);
	vector<PxRigidStatic*> Add_Static_Mesh_Actor(const PHYSX_INIT_DESC& Desc);
	vector<PxRigidDynamic*> Add_Dynamic_Mesh_Actor(const PHYSX_INIT_DESC& Desc);

	HRESULT Add_Ground(class CGameObject* pGroundObj);

public:
	HRESULT Reset_PhysX();

public:
	HRESULT Remove_Actor(_uint iObjectID, PHYSX_RIGID_TYPE eRigidType);

public:
	HRESULT Convert_Transform(class CGameObject* pObj, PxTransform& PxTransform);

public:
	PxMaterial* Create_Material(_float fStaticFriction, _float fDynamicFriction, _float fRestitution) {
		return m_Physics->createMaterial(fStaticFriction, fDynamicFriction, fRestitution);
	}

public:
	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override;
	virtual void onWake(PxActor** actors, PxU32 count) override;
	virtual void onSleep(PxActor** actors, PxU32 count) override;
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
	virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override;


private:
	PxRigidDynamic* Create_Dynamic_Box(const PHYSX_INIT_DESC& Desc);
	PxRigidDynamic* Create_Dynamic_Sphere(const PHYSX_INIT_DESC& Desc);
	vector<PxRigidDynamic*> Create_Dynamic_Mesh(const PHYSX_INIT_DESC& Desc);

	PxRigidStatic* Create_Static_Box(const PHYSX_INIT_DESC& Desc);
	PxRigidStatic* Create_Static_Sphere(const PHYSX_INIT_DESC& Desc);
	vector<PxRigidStatic*> Create_Static_Mesh(const PHYSX_INIT_DESC& Desc);





#ifdef _DEBUG
private:
	PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;
	BasicEffect* m_pEffect = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
#endif

private:
	PxDefaultAllocator			m_Allocator;			// �޸� ������..?
	PxDefaultErrorCallback		m_ErrorCallback;		// ���鶧 �ʿ���.
	

	PxFoundation*				m_Foundation = nullptr;		// ���� ����
	PxPhysics*					m_Physics = nullptr;		// �ھ �Ǵ°� ����.

	PxDefaultCpuDispatcher*		m_Dispatcher = nullptr;		// ��Ƽ������ ����?
	PxScene*					m_pScene = nullptr;			// �ùķ��̼� ���� Scene.

	PxMaterial* m_WorldMaterial = nullptr;		// ��ü�� ����
	PxCudaContextManager*       m_pCudaContextManager = nullptr;


	PxPvd*						m_pPvd = nullptr; // ����
	string						m_strIPAddress = "127.0.0.1";
	int							m_iPortNumber = 5555;
	_uint						m_iTimeOutSeconds = 10;


private:
	map<_uint, PHYSX_STATIC_OBJECT_DESC> m_StaticObjects;
	map<_uint, PHYSX_DYNAMIC_OBJECT_DESC> m_DynamicObjects;
	map<_uint, PHYSX_STATIC_OBJECT_DESC> m_GroundObjects;

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

		
public:
	virtual void Free() override;





};

END