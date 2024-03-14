#pragma once

#include "Base.h"
#include "Engine_Defines.h"


BEGIN(Engine)

class CPhysX_Manager final : public CBase, public PxSimulationEventCallback, public PxControllerFilterCallback
{
	DECLARE_SINGLETON(CPhysX_Manager)
	
	
public:
	CPhysX_Manager();
	virtual ~CPhysX_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);



public:
	HRESULT Add_Ground(class CGameObject* pGameObject, class CModel* pModel, Matrix WorldMatrix, const wstring& strCollisionTag);
	HRESULT Add_Building(class CGameObject* pGameObject, class CModel* pModel, Matrix WorldMatrix, const wstring& strCollisionTag);
	PxController* Add_CapsuleController(class CGameObject* pGameObject, Matrix WorldMatrix, _float fHeight, _float fRadius, _float3 vOffsetPos, _float fMaxJumpHeight, PxUserControllerHitReport* pCallBack);
	PxController* Add_BoxController(CGameObject* pGameObject, Matrix WorldMatrix, _float3 fExtents, _float3 vOffsetPos, _float fMaxJumpHeight,  PxUserControllerHitReport* pCallBack);


	HRESULT Clear_PhysX_Ground();

public:
	HRESULT Reset_PhysX();

public:
	HRESULT Remove_Controller(PxController* pController);
	HRESULT Remove_Actor(class CGameObject* pGameObject);
	
public:
	void Is_Valid_Scene();

	

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

public:
	// PxControllerFilterCallback��(��) ���� ��ӵ�
	_bool Check_Push(_uint iLeftObjType, _uint iRightObjType);
	virtual bool filter(const PxController& a, const PxController& b) override;


private:
	PxDefaultAllocator			m_Allocator;			// �޸� ������..?
	PxDefaultErrorCallback		m_ErrorCallback;		// ���鶧 �ʿ���.
	

	PxFoundation*				m_Foundation = nullptr;		// ���� ����
	PxPhysics*					m_Physics = nullptr;		// �ھ �Ǵ°� ����.

	PxDefaultCpuDispatcher*		m_Dispatcher = nullptr;		// ��Ƽ������ ����?
	PxScene*					m_pScene = nullptr;			// �ùķ��̼� ���� Scene.

	PxMaterial* m_WorldMaterial = nullptr;		// ��ü�� ����
	PxControllerManager* m_pController_Manager = nullptr; // ��Ʈ�ѷ� �Ŵ���

private:
	map<_uint, vector<PHYSX_STATIC_OBJECT_DESC>> m_GroundObjects;
	vector<OBJ_TYPE> m_eObjectTypes;

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	_float m_fAccTime = 0.f;


		
public:
	virtual void Free() override;

	
};

END