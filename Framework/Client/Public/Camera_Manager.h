#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Camera.h"

BEGIN(Engine)
class CCamera;
class CGameObject;
class CTransform;
END

BEGIN(Client)
class CCamera_Manager : public CBase
{
	DECLARE_SINGLETON(CCamera_Manager)

private:
	CCamera_Manager();
	virtual ~CCamera_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public:
	/* Access */
	CCamera* Get_Camera(const CAMERA_TYPE& eType);
	CCamera* Get_CurCamera() const { return m_pCurCamera; }

	HRESULT Set_CurCamera(const CAMERA_TYPE& eType);

	const _bool Is_Empty_Camera(const CAMERA_TYPE& eType) { return (nullptr == m_Cameras[eType]) ? true : false; }

public:
	/* Event */
	/* ī�޶� ���̽� Ŭ�������� ���� �ִ� �����ͷ� ���, �ַ� �ִϸ��̼� �̺�Ʈ���� ��� */
	HRESULT Start_Action_Shake(const CAMERA_EVENT_DESC& tDesc);
	HRESULT Start_Action_Lerp_Fov(const CAMERA_EVENT_DESC& tDesc);
	HRESULT Start_Action_Lerp_Dist(const CAMERA_EVENT_DESC& tDesc);

	HRESULT Start_Action_Shake_Default();

	/* �ƽ� ī�޶� */
	HRESULT Start_Action_CutScene(const string& strCutSceneName);

private:
	/* Cameras */
	CCamera* m_pCurCamera	= { nullptr };
	CCamera* m_pNextCamera	= { nullptr };
	CCamera* m_Cameras[CAMERA_TYPE::CAMERA_TYPE_END];


private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	HRESULT Ready_Cameras();

public:
	virtual void Free() override;
};

END
