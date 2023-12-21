#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

#pragma region Access Methods 
public: 
	/* Model Prop */
	void Set_Name(const wstring& strName) { m_strName = strName; }
	void Set_CustomSocketPivotRotation(const _uint iIndex, Vec3 vCustomSocket);

	TYPE Get_ModelType() { return m_eModelType; }
	Vec3 Get_CustomSocketPivotRotation(const _uint iIndex); /* �������� ����ϴ� Ŀ���� �ǹ� ��Ʈ���� */
	_matrix Get_PivotMatrix() { return XMLoadFloat4x4(&m_PivotMatrix); }
	const wstring& Get_Name() const { return m_strName; }

	void Add_CustomSocketPivotRotation(Vec3 vCustomSocket) { m_SocketCustomPivotRotation.push_back(vCustomSocket); }

	/* HierarchyNode */
	Matrix Get_SocketLocalMatrix(const _uint iSocketEnumIndex); /* (���� Latetick ���� ȣ��)ĳ���Ͱ� ���� �ִ� ������ ���� ��Ʈ������ �����Ѵ�. (ĳ���Ϳ� ���ǵ� enum �ε��� ���, �� ��ȣ �ƴ�) */
	wstring Get_HiearachyNodeName(const _uint iIndex);
	const _int Get_HierarchyNodeIndex(const char* szBonename);
	const _int Get_HierarchyNodeIndex(wstring strBoneName);
	class CHierarchyNode* Get_HierarchyNode(const wstring & strNodeName);
	class CHierarchyNode* Get_HierarchyNode(const _uint iIndex);
	vector<class CHierarchyNode*>& Get_HierarchyNodes() { return m_HierarchyNodes; }

	/* Meshes */
	_uint Get_NumMeshes() const { return m_iNumMeshes; }
	const vector<class CMesh*>& Get_Meshes() { return m_Meshes; }

	/* Matertial */
	_uint Get_MaterialIndex(_uint iMeshIndex);
	class CTexture* Get_MaterialTexture(_uint iMeshIndex, _uint iTextureType);

	/* Animation */
	const _bool Has_Animation(const wstring& strAnimationName);
	
	void Set_Stop_Animation(const _bool& bStop) { m_TweenDesc.cur.iStop = bStop; } /* bStop�� Ʈ���� �� ����Ʈƽ�� ȣ��Ǵ��� �ִϸ��̼��� ���ŵ��� �ʴ´�. */
	void Set_KeyFrame_By_Progress(_float fProgress); /* �����(0 ~ 1)�� �Ű������� �޾� Ű�������� �����Ѵ�. */
	HRESULT Set_Animation(const _uint& iAnimationIndex, const _float& fTweenDuration = DEFAULT_TWEEN_DURATION); /* �ε����� �ִϸ��̼� �÷��� (fTweenDuration�� ������� ���� X) */
	HRESULT Set_Animation(const wstring& strAnimationName, const _float& fTweenDuration = DEFAULT_TWEEN_DURATION); /* �̸����� �ִϸ��̼� �÷��� (fTweenDuration�� ������� ���� X) */
	
	_uint Get_CurrAnimationIndex() { return m_TweenDesc.cur.iAnimIndex; } /* ���� �ִϸ��̼��� �ε��� ����*/
	class CAnimation* Get_Animation(const _uint& iIndex);
	class CAnimation* Get_Animation(const string strName);
	const _int Get_AnimationIndex(const wstring& strName);
	const _uint& Get_CurrAnimationFrame() const { return m_TweenDesc.cur.iCurFrame; } /* ���� �ִϸ��̼��� ������ ���� (���� ����) */
	const _float Get_CurrAnimationFrame_WithRatio() { return (m_TweenDesc.cur.iCurFrame + m_TweenDesc.cur.fRatio); } /* ���� �ִϸ��̼��� ������(���� ����) (�Ǽ� ����) */
	const _float Get_Progress();  /* ���� �ִϸ��̼��� �����(0~1) ����*/
	const TweenDesc& Get_TweenDesc() const { return m_TweenDesc; } /* ����, ���� �ִϸ��̼� ���� ����ü ���� */
	class CAnimation* Get_CurrAnimation() { return m_Animations[m_TweenDesc.cur.iAnimIndex]; } /* ���� �ִϸ��̼� ��ü ���� */
	vector<class CAnimation*>& Get_Animations() { return m_Animations; } /* ��ü �ִϸ��̼� �����̳� ����*/

	_int Find_AnimationIndex(const wstring& strAnimationTag); /* �̸��� Ű�� ����ؼ� �ִϸ��̼��� �ε��� ���� */

	const _bool Is_Half() const { return (0.5f <= m_TweenDesc.cur.fRatio) ? true : false; } /* ���� �ִϸ��̼��� �� �̻� ����ƴ��� ���� ���� */
	const _bool Is_Finish() const { return m_TweenDesc.cur.iFinish; } /* ���� �ִϸ��̼��� ����ƴ��� ���� (����Ǿ��µ� ���� ���� �ִϸ��̼��� ���� �ȵǾ� �ִٸ� ���� ���� ����) */
	const _bool Is_Tween() const { return (0 <= m_TweenDesc.next.iAnimIndex) ? true : false; } /* �ִϸ��̼� Ʈ���� (���� �ִϸ��̼� ����) ���� ���� */
	const _bool Is_Fix() const { return m_TweenDesc.cur.iFix; } /* ���� �ִϸ��̼��� ������ �����ӿ��� ���� �������� ���� ����*/
	const _bool Is_Stop() const { return m_TweenDesc.cur.iStop; } /* ���� �ִϸ��̼� ���� ���� */

	void Debug_Animation();
#pragma endregion

#pragma region Life Cycle
public:
	virtual HRESULT Initialize_Prototype(TYPE eType, const wstring & strModelFilePath, const wstring & strModelFileName, _fmatrix PivotMatrix); // << : Assimp 
	virtual HRESULT Initialize_Prototype_Part(const wstring& strModelFilePath, const wstring& strModelFileName, _fmatrix PivotMatrix, class CModel* pCharacterModel); // << : Assimp 
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Bin(void* pArg);

	HRESULT LateTick(_float fTimeDelta); /* ���� �ִϸ��̼� Ű������ ������Ʈ (���� �ڵ忡���� PlayAnimation() �Լ�?) */

	HRESULT SetUp_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pConstantName);
	HRESULT SetUp_VTF(class CShader* pShader);
	HRESULT Render(class CShader* pShader, _uint iMeshIndex, _uint iPassIndex = 0);
	HRESULT Render_Instancing(class CShader* pShader, _uint iMeshIndex, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, _uint iPassIndex = 0);
#pragma endregion

#pragma region ImGui Tool
public:
	const aiScene* Get_Scene() { return m_pAIScene; }
	
	void Add_SocketTransforms(vector<ANIM_TRANSFORM_CACHE> SocketTransforms) { m_SocketTransforms.push_back(SocketTransforms); }

	void Clear_SocketTransformsCache(const _uint iSocketIndex);
	void Clear_All_SocketTransformsCaches();
	void Add_SocketTransformIndexCache(_uint iIndex) { m_SocketTransformIndexCache.push_back(iIndex); }
	vector<_uint> Get_SocketTransformIndexCache() { return m_SocketTransformIndexCache; }

	HRESULT Delete_Animation(_uint iIndex);
	HRESULT Swap_Animation(_uint iSrcIndex, _uint iDestIndex);

#pragma endregion

#pragma region Vtf
public:
	HRESULT Set_VtfSrv(ID3D11ShaderResourceView* pSrv);
	HRESULT Clear_NotUsedData();
#pragma endregion

private:
	wstring m_strName;
	wstring m_strFileName;
	wstring m_strFolderPath;

private:
	const aiScene* m_pAIScene = nullptr;
	Assimp::Importer m_Importer;

	TYPE m_eModelType = TYPE_END;
	_bool m_bFromBinary = false;
	_float4x4 m_PivotMatrix;

private:
	vector<class CHierarchyNode*> m_HierarchyNodes;

	_uint m_iNumMeshes = 0;
	vector<class CMesh*> m_Meshes;
	typedef vector<class CMesh*> MESHES;

	_uint m_iNumMaterials = 0;
	vector<MATERIALDESC> m_Materials;

	_uint m_iNumAnimations = 0;
	vector<class CAnimation*> m_Animations;

private:
	TWEEN_DESC m_TweenDesc = {};
	ID3D11ShaderResourceView* m_pSRV = nullptr;

	vector<_uint> m_SocketTransformIndexCache;
	vector<Vec3> m_SocketCustomPivotRotation;
	vector<vector<ANIM_TRANSFORM_CACHE>> m_SocketTransforms; /* ����(��) ���� vector<ANIM_TRANSOFRM_CACHE>�� ���´�.*/

#pragma region Assimp
private:
	HRESULT Ready_MeshContainers(_fmatrix PivotMatrix);
	HRESULT Ready_MeshContainers_Part(_fmatrix PivotMatrix, class CModel* pCharacterModel);
	HRESULT Ready_Materials(const wstring & pModelFilePath);
	HRESULT Ready_HierarchyNodes(aiNode * pNode, class CHierarchyNode* pParent, _uint iDepth);
	HRESULT Ready_Animations();
#pragma endregion

private:
	static CModel* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, TYPE eType, const wstring & strModelFilePath, const wstring & strModelFileName, _fmatrix PivotMatrix = XMMatrixIdentity());
	static CModel* Create_Bin(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, TYPE eType, const wstring & strModelFilePath, const wstring & strModelFileName, _fmatrix PivotMatrix = XMMatrixIdentity());
	static CModel* Create_Part(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const wstring& strModelFilePath, const wstring& strModelFileName, CModel* pCharacterModel, _fmatrix PivotMatrix = XMMatrixIdentity());

public:
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;

public:
	friend class CModel_Manager;
};



END