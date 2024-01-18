#pragma once

#include "Client_Defines.h"
#include "Camera.h"

#include "Camera_Manager.h"

BEGIN(Client)

typedef struct tagCameraCutSceneMapDesc
{
	string		strCutSceneName = {};

	Vec3		vCamPositions[MAX_BEZIER_POINT];
	Vec3		vCamLookAts[MAX_BEZIER_POINT];

	_float		fDuration			= 4.f;
	_float		fStartDelayTime		= 0.f;
	_float		fFinishDelayTime	= 0.f;

	_float		fStartFov			= 0.f; // ���� �̻�� 
	_float		fFinishFov			= 0.f;

	LERP_MODE	eLerpMode = LERP_MODE::SMOOTHER_STEP;

}CAMERA_CUTSCENE_MAP_DESC;

class CCamera_CutScene_Map final : public CCamera
{
private:
	CCamera_CutScene_Map(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera_CutScene_Map(const CCamera_CutScene_Map& rhs);
	virtual ~CCamera_CutScene_Map() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Tick_Blending(const _float fDeltaTime) override;

public:
	HRESULT Start_CutScene(const string& strCutSceneName,
							const _bool& bWillRetruePrevCam = false,
							const _bool& bWillBlending = false, 
							const _uint& iBlendingCamKey = 0,
							const _float& fBlendingDuration = 1.f, 
							const LERP_MODE& eBlendingMode = LERP_MODE::SMOOTHER_STEP); /* ���� �ƽ� ���� */

	HRESULT Start_CutScenes(vector<string> strCutSceneNames,
							const _bool& bWillRetruePrevCam = false,
							const _bool& bWillBlending = false,
							const _uint& iBlendingCamKey = 0,
							const _float& fBlendingDuration = 1.f,
							const LERP_MODE& eBlendingMode = LERP_MODE::SMOOTHER_STEP); /* ���� �ƽ� ���� */

	HRESULT Stop_CutScene(const _bool& bClearReservedCutScene = false);

	const _bool Is_Playing_CutScenc() const { return m_tTimeDesc.bActive; }

public:
	static Vec4 Get_Point_In_Bezier(Vec3 vPoints[MAX_BEZIER_POINT], const _float& fRatio);

	virtual Vec4 Get_LookAt() override;

public:
	HRESULT Save_CutSceneDescs();
	HRESULT Load_CutSceneDescs();

public:
	/* Tool */
	HRESULT	Add_CutSceneDesc(const CAMERA_CUTSCENE_MAP_DESC& desc);
	HRESULT Del_CutSceneDesc(const string& strCutSceneName);
	HRESULT Change_CutSceneDesc(const _int& iIndex, const CAMERA_CUTSCENE_MAP_DESC& desc);
	CAMERA_CUTSCENE_MAP_DESC* Find_CutSceneDesc(const string& strCutSceneName);
	const vector<CAMERA_CUTSCENE_MAP_DESC>& Get_CutSceneDescs() const { return m_CutSceneDescs; }

private:
	virtual HRESULT Ready_Components() override;

private:
	vector<CAMERA_CUTSCENE_MAP_DESC>	m_CutSceneDescs;

	CAMERA_CUTSCENE_MAP_DESC*			m_pCurCutSceneDesc = nullptr;

	queue<string>						m_CutSceneNamesReserved;

	LERP_TIME_DESC						m_tTimeDesc;
	
	/* �ƽ� ���� ü���� ���� ������ */
	_bool								m_bWillRetruePrevCam = false;
	_bool								m_bWillBlending = false;
	_uint								m_iBlendingCamKey = 0;
	_float								m_fBlendingDuration = 0.f;
	LERP_MODE							m_eBlendingMode = LERP_MODE::SMOOTHER_STEP;

	
public:
	static CCamera_CutScene_Map* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END