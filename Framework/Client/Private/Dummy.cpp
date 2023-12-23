#include "stdafx.h"
#include "Dummy.h"
#include "GameInstance.h"

#include "Camera_Manager.h"

CDummy::CDummy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_DUMMY)
{
}

CDummy::CDummy(const CDummy& rhs)
	: CGameObject(rhs)
{

}

HRESULT CDummy::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CDummy::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (!CCamera_Manager::GetInstance()->Is_Empty_Camera(CAMERA_TYPE::FOLLOW))
	{
		CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_TargetObj(this);
		CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW)->Set_LookAtObj(this);
	}
	return S_OK;
}

HRESULT CDummy::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_AnimShader"), (CComponent**)&m_pAnimShaderCom)))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_NonAnim_Shader"), (CComponent**)&m_pNonAnimShaderCom)))
			return E_FAIL;
	}

	return S_OK;
}


void CDummy::Tick(_float fTimeDelta)
{
	Input(fTimeDelta);
}

void CDummy::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom || nullptr == m_pModelCom)
		return;

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	if (CModel::TYPE::TYPE_ANIM == m_pModelCom->Get_ModelType())
		m_pModelCom->LateTick(fTimeDelta);
}

HRESULT CDummy::Render()
{
	if (nullptr == m_pModelCom)
		return S_OK;

	/* ���� ������ */
	CShader* pShader = m_pModelCom->Get_ModelType() == CModel::TYPE::TYPE_NONANIM ? m_pNonAnimShaderCom : m_pAnimShaderCom;

	if (nullptr == pShader)
		return S_OK;

	if (FAILED(pShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	/* �ڽ�Ƭ �۾��� */
	if (m_bCostumeMode)
	{
		if (FAILED(m_pModelCom->SetUp_VTF(pShader)))
			return E_FAIL;
		for (size_t i = 0; i < PART_TYPE::PART_END; i++)
		{
			if (nullptr == m_pPart[i])
				continue;

			const _uint		iNumMeshes = m_pPart[i]->Get_NumMeshes();

			for (_uint j = 0; j < iNumMeshes; ++j)
			{
				if (FAILED(m_pPart[i]->SetUp_OnShader(pShader, m_pPart[i]->Get_MaterialIndex(j), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
					return E_FAIL;

				if (FAILED(m_pPart[i]->Render(pShader, j)))
					return E_FAIL;
			}
		}

		return S_OK;
	}

	/* �ڽ�Ƭ �۾� �ƴҽ� */
	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	if (0 < m_pModelCom->Get_Animations().size() && m_pModelCom->Get_ModelType() == CModel::TYPE_ANIM)
	{
		if (FAILED(m_pModelCom->SetUp_VTF(pShader)))
			return E_FAIL;
	}

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(pShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(pShader, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CDummy::Ready_ModelCom(_uint eType, const wstring& strFilePath, const wstring& strFileName)
{
	if (nullptr != m_pModelCom)
		Safe_Release(m_pModelCom);	

	_tchar szFileName[MAX_PATH];
	_tchar szExt[MAX_PATH];
	_wsplitpath_s(strFileName.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

	if (FAILED(GI->Import_Model_Data(LEVEL_DUMMY, wstring(L"Prototype_Componenet_Model_") + szFileName, eType, strFilePath, strFileName, &m_pModelCom)))
		return E_FAIL;


	m_pModelCom->Set_Owner(this);

	return S_OK;
}


HRESULT CDummy::Export_Model_Bin(const wstring& strFilePath, const wstring& strFileName)
{
	if (nullptr == m_pModelCom)
		return E_FAIL;
	
	if (FAILED(GI->Export_Model_Data(m_pModelCom, strFilePath, strFileName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDummy::Import_Model()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	return S_OK;
}

void CDummy::Input(_float fTimeDelta)
{

}


CDummy* CDummy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CDummy* pInstance = new CDummy(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CDummy");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CDummy::Clone(void* pArg)
{
	CDummy* pInstance = new CDummy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDummy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDummy::Free()
{
	__super::Free();

	Safe_Release(m_pNonAnimShaderCom);
	Safe_Release(m_pAnimShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	// Safe_Release(m_pModelCom);
}


