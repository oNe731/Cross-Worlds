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
class CRigidBody;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CEffect final : public CGameObject
{
public:
	enum EFFECT_TYPE { EFFECT_TEXTURE, EFFECT_MESH, EFFECT_END };

public:
	typedef struct tagEffectDesc // 반복X 자체적으로 계속 생성
	{	
		// 이펙트 타입
		EFFECT_TYPE eType = EFFECT_TYPE::EFFECT_MESH;

		// 중력 
		_bool bGravity = false; // -> 추후 필요시 기능 구현

		// 위치
		_float3 fRange         = _float3(0.f, 0.f, 0.f);
		_float2 fRangeDistance = _float2(0.f, 0.f);

#pragma region 크기
		_bool   bScaleSameRate = true; // 정비율
		_float3 fScaleStartMin = _float3(2.f, 2.f, 2.f);
		_float3 fScaleStartMax = _float3(2.f, 2.f, 2.f);

		_bool   bScaleChange           = false;
		_float2 fScaleChangeStartDelay = _float2(0.f, 0.f);

		_bool   bScaleChangeRandom = false;
		_float2 fScaleChangeTime   = _float2(1.f, 5.f);

		_bool   bScaleAdd       = false;
		_bool   bScaleLoop      = false;
		_bool   bScaleLoopStart = false;

		_float3 fScaleSizeMin  = _float3(1.f, 1.f, 1.f);
		_float3 fScaleSizeMax  = _float3(5.f, 5.f, 5.f);
		_float2	fScaleSpeed    = _float2(1.f, 1.f);
		_float3	fScaleDirSpeed = _float3(1.f, 1.f, 1.f);
#pragma endregion

#pragma region 이동
		_float2	fVelocitySpeed = _float2(0.f, 0.f);

		_float3 vVelocityMinStart = _float3(-1.0f, -1.0f, -1.0f);
		_float3 vVelocityMaxStart = _float3(1.0f, 1.0f, 1.0f);

		_bool   bVelocityChange           = false;
		_float2 fVelocityChangeStartDelay = _float2(0.f, 0.f);
		_float2 fVelocityChangeTime       = _float2(1.f, 5.f);
#pragma endregion

#pragma region 회전
		_bool	bBillboard = false;

		_bool   bRandomAxis = false;
		_float3 fAxis       = _float3(0.f, 1.f, 0.f);

		_bool  bRandomAngle = false;
		_float fAngle = 0.f;

		_bool	bRotationChange = false;
		_float2 fRotationChangeStartDelay = _float2(0.f, 0.f);

		_float2 fRotationSpeed = _float2(30.f, 30.f);

		_float3 fRotationDir = _float3(0.f, 1.f, 0.f);

		_bool   bRotationChangeRandom = false;
		_float2 fRotationChangeTime = _float2(1.f, 1.f);
#pragma endregion

		// 지속 시간
		_float2 fLifeTime = _float2(100.f, 100.f);

#pragma region 모델 && 텍스처
		wstring strModelName            = L"Prototype_Component_Model_PS_NOTE_CIRCLE01";
		wstring strDiffuseTetextureName = L"";
		wstring strAlphaTexturName      = L"Prototype_Component_Texture_Effect_Aura";

		_int    iTextureIndexDiffuse = 0;
		_int    iTextureIndexAlpha   = 0;

		// SequenceTexture : Diffuse && Alpha
		_bool   bRandomStartIndex = false;
		_float2	fUVIndex  = _float2(0.f, 0.f);
		_float2	fMaxCount = _float2(1.f, 1.f);

		// UVFlow
		_bool   bUVFlowChange = true;
		_int	iUVFlowLoop  = -1; // 0 < UVLoop -> NoLoop
		_float2	fUVFlowDir   = _float2(0.f, 0.f);
		_float2	fUVFlowSpeed = _float2(0.f, 0.f);
#pragma endregion

#pragma region 애니메이션
		_bool   bAnimation      = false;
		_bool   bAnimationLoop  = true;
		_bool   bIncrement      = true;
		_float2 fAnimationSpeed = _float2(10.f, 10.f);
#pragma endregion

#pragma region 알파
		_float2 fAlphaStart = _float2(1.f, 1.f);

		_bool bAlphaCreate = false;
		_bool bAlphaDelete = false;
		_float2 fAlphaSpeed = _float2(0.f, 0.f);

		_bool bAlphaChange = false;
		_bool bAlphaIn     = false;
		_float2 fAlphaChangeStartDelay = _float2(0.f, 0.f);
#pragma endregion

#pragma region 색상
		_bool   bColorRandom = false;
		_float4	fColorS = _float4(1.f, 1.f, 1.f, 1.f);

		_bool bColorChange = false;

		_bool bColorChangeRandom = false;
		_float2 fColorChangeRandomTime = _float2(4.f, 4.f);

		_bool bColorLoop = false;
		_float2 fColorChangeStartDelay = _float2(2.f, 2.f);

		_float2 fColorChangeStartM = _float2(4.f, 4.f);
		_float4 fColorM = _float4(0.f, 1.f, 0.f, 0.f);

		_float2 fColorChangeStartF = _float2(6.f, 6.f);
		_float4 fColorF = _float4(0.f, 0.f, 1.f, 0.f);

		// 보간 시간
		_float2 fColorDuration = _float2(2.f, 2.f);
#pragma endregion

#pragma region 블러
		_bool   bBloomPowerRandom = false;
		_float4	fBloomPower      = _float4(1.f, 0.f, 0.f, 1.f);
		_bool   bBlurPowerRandom = false;
		_float	fBlurPower       = 0.5f;
#pragma endregion

#pragma region 기타 정보
		_uint   iShaderPass    = 0;
		_float  fAlpha_Discard = 0.5f;
		_float3 fBlack_Discard = _float3(0.5f, 0.5f, 0.5f);
#pragma endregion

		// 생성 위치 관련
		_float4x4 OffsetMatrix;

		tagEffectDesc()
		{
			XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());
		}

		// 복사 생성자 내용 수정 전부 추가
		tagEffectDesc(const tagEffectDesc& rhs)
		{
			eType    = rhs.eType;
			bGravity = rhs.bGravity;
			//
			fRange         = rhs.fRange;
			fRangeDistance = rhs.fRangeDistance;
			//
			bScaleSameRate = rhs.bScaleSameRate;
			fScaleStartMin = rhs.fScaleStartMin;
			fScaleStartMax = rhs.fScaleStartMax;

			bScaleChange           = rhs.bScaleChange;
			fScaleChangeStartDelay = rhs.fScaleChangeStartDelay;

			bScaleChangeRandom = rhs.bScaleChangeRandom;
			fScaleChangeTime   = rhs.fScaleChangeTime;

			bScaleAdd       = rhs.bScaleAdd;
			bScaleLoop      = rhs.bScaleLoop;
			bScaleLoopStart = rhs.bScaleLoopStart;

			fScaleSizeMin      = rhs.fScaleSizeMin;
			fScaleSizeMax      = rhs.fScaleSizeMax;
			fScaleSpeed    = rhs.fScaleSpeed;
			fScaleDirSpeed = rhs.fScaleDirSpeed;
			//
			fVelocitySpeed = rhs.fVelocitySpeed;

			vVelocityMinStart = rhs.vVelocityMinStart;
			vVelocityMaxStart = rhs.vVelocityMaxStart;

			bVelocityChange           = rhs.bVelocityChange;
			fVelocityChangeStartDelay = rhs.fVelocityChangeStartDelay;
			fVelocityChangeTime       = rhs.fVelocityChangeTime;
			//


			bBillboard = rhs.bBillboard;
			bRotationChange = rhs.bRotationChange;
			fRotationDir = rhs.fRotationDir;
			fRotationSpeed = rhs.fRotationSpeed;
			fLifeTime = rhs.fLifeTime;
			strModelName = rhs.strModelName;
			strDiffuseTetextureName = rhs.strDiffuseTetextureName;
			strAlphaTexturName = rhs.strAlphaTexturName;
			iTextureIndexDiffuse = rhs.iTextureIndexDiffuse;
			iTextureIndexAlpha = rhs.iTextureIndexAlpha;
			bRandomStartIndex = rhs.bRandomStartIndex;
			fUVIndex = rhs.fUVIndex;
			fMaxCount = rhs.fMaxCount;
			bUVFlowChange = rhs.bUVFlowChange;
			iUVFlowLoop = rhs.iUVFlowLoop;
			fUVFlowDir = rhs.fUVFlowDir;
			bAnimation = rhs.bAnimation;
			bAnimationLoop = rhs.bAnimationLoop;
			bIncrement = rhs.bIncrement;
			fAnimationSpeed = rhs.fAnimationSpeed;
			fAlphaStart = rhs.fAlphaStart;
			bAlphaCreate = rhs.bAlphaCreate;
			bAlphaDelete = rhs.bAlphaDelete;
			fAlphaSpeed = rhs.fAlphaSpeed;
			bAlphaChange = rhs.bAlphaChange;
			bAlphaIn = rhs.bAlphaIn;
			fAlphaChangeStartDelay = rhs.fAlphaChangeStartDelay;
			bColorRandom = rhs.bColorRandom;
			fColorS      = rhs.fColorS;
			fBlurPower   = rhs.fBlurPower;
			fBloomPower  = rhs.fBloomPower;
			iShaderPass  = rhs.iShaderPass;
		    OffsetMatrix = rhs.OffsetMatrix;
		}

	} EFFECT_DESC;

protected:
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	virtual HRESULT Initialize_Prototype(const EFFECT_DESC* tEffectDesc);
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices) override;

public:
	const EFFECT_DESC& Get_EffectDesc() { return m_tEffectDesc; }
	void Set_EffectDesc(const EFFECT_DESC& tDesc);
	void Reset_Effect();
	void Reset_UV();

public:
	void Set_Owner(CGameObject* pGameObject) { m_pOwnerObject = pGameObject; }
	void Set_OffsetMatrix(_fmatrix fOffsetMatrix) { XMStoreFloat4x4(&m_tEffectDesc.OffsetMatrix, fOffsetMatrix); }
	const _float4x4& Get_OffsetMatrix() { return  m_tEffectDesc.OffsetMatrix; }
	void Set_ParentMatrix(_matrix ParentMatrix) { XMStoreFloat4x4(&m_ParentMatrix, ParentMatrix); }

public:
	_bool Is_End() { return m_bAccIndexEnd; };
	void Set_End(_bool bEnd) { m_bAccIndexEnd = bEnd; }
	void Set_Gravity(_bool bGravity);

public:
	class CTexture* Get_DiffuseTexture() { return m_pDiffuseTextureCom; }
	class CTexture* Get_AlphaTexture()   { return m_pAlphaTextureCom; }
	class CTransform* Get_TransformCom() { return m_pTransformCom; }
	class CRigidBody* Get_RigidBodyCom() { return m_pRigidBodyCom; }

private:
	EFFECT_DESC m_tEffectDesc;

private:
	CGameObject* m_pOwnerObject = nullptr;
	_float4x4    m_ParentMatrix;

private:
	_bool   m_bEffectDie = false;

	_float  m_fAccDeletionTime = 0.f;

	// 스케일
	_float m_fScaleSpeed    = 0.f;
	_float m_fScaleTimeAccs = 0.f;
	_float m_fScaleChange   = 0.f;
	_float m_fScaleChangeStartTime  = 0.f;
	_float m_fScaleChangeStartDelay = 0.f;

	// 이동
	_float3	m_fVelocity      = _float3(0.f, 0.f, 0.f);
	_float  m_fVelocitySpeed = 0.f;
	_float  m_fVelocityChangeStartTime  = 0.f;
	_float  m_fVelocityChangeStartDelay = 0.f;
	_float  m_fVelocityRanTimeAccs      = 0.f;
	_float  m_fVelocityRanChange        = 0.f;

	// 회전
	_float3 m_fAxis  = _float3(0.f, 0.f, 0.f);
	_float  m_fAngle = 0.f;
	_float m_fRotationSpeed = 0.f;
	_float m_fRotationChangeStartTime  = 0.f;
	_float m_fRotationChangeStartDelay = 0.f;
	_float m_fRotationTime             = 0.f;
	_float m_fRotationChangeTime       = 0.f;

	// 지속 시간
	_float m_fLifeTime = 0.f;

	// 애니메이션
	_bool   m_bAccIndexEnd    = false;
	_float  m_fAccIndex       = 0.f;
	_float  m_fAnimationSpeed = 0.f;
	_float2 m_fAccUVFlow   = _float2(0.f, 0.f);
	_float 	m_fUVFlowSpeed = 0.f;

	// 알파
	_float m_fAlpha      = 1.f;
	_float m_fAlphaSpeed = 0.f;
	_float m_fAlphaChangeStartTime  = 0.f;
	_float m_fAlphaChangeStartDelay = 0.f;
	_bool  m_bAlphaCreateSucc       = false;

	// 색상
	_float  m_fColorChangeStartTime;
	_float  m_fColorChangeStartDelay;

	LERP_VEC3_DESC m_LerpInfo;
	_float3 m_fColor;
	_uint   m_iColorIndex;
	_float  m_fColorChangeStartM;
	_float  m_fColorChangeStartF;
	_float  m_fColorChangeDuration;

	_float  m_fColorAccs;
	_float  m_fColorChangeTime;
	_float3 m_fNextColor;
	_bool   m_fColorFade;

	_float3 m_fBlurColor;
	_float  m_fBlurPower;

private:
	class CRenderer*  m_pRendererCom  = nullptr;
	class CTransform* m_pTransformCom = nullptr;
	class CRigidBody* m_pRigidBodyCom = nullptr;
	class CModel*         m_pModelCom = nullptr;
	class CVIBuffer_Rect* m_pVIBufferCom = nullptr;
	class CTexture* m_pDiffuseTextureCom = nullptr;
	class CTexture* m_pAlphaTextureCom = nullptr;

private:
	HRESULT Bind_ShaderResource();
	HRESULT Bind_ShaderResource_Instance(class CShader* pShader);

	/* For.Texture_Effect */
	virtual void Increment(_float fTimeDelta);
	virtual void Decrement(_float fTimeDelta);

	void Change_Scale(_float fTimeDelta);
	void Change_Move(_float fTimeDelta);
	void Change_Rotation(_float fTimeDelta);
	void Change_Alpha(_float fTimeDelta);
	void Change_Color(_float fTimeDelta);

	void Set_Model();
	void Set_Texture_Diffuse();
	void Set_Texture_Alpha();

protected:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Components() override;

public:
	static CEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, 
		const wstring& strObjectTag, const EFFECT_DESC* tEffectDesc = nullptr, const wstring& strEffectFilePath = {});
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END


