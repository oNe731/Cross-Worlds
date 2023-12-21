#include "stdafx.h"
#include "GameInstance.h"
#include "Part_Manager.h"
#include "Model.h"
#include "Animation.h"
#include "Utils.h"

IMPLEMENT_SINGLETON(CPart_Manager)

CPart_Manager::CPart_Manager()
{

}

HRESULT CPart_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	if (FAILED(Ready_PartModels()))
		return E_FAIL;

	return S_OK;
}

CModel* CPart_Manager::Get_PartModel(const CHARACTER_TYPE& eCharacterType, const PART_TYPE& ePartType, const _uint iIndex)
{
	if (CHARACTER_TYPE::CHARACTER_END <= eCharacterType || PART_TYPE::PART_END < ePartType || eCharacterType < 0 || ePartType < 0)
		return nullptr;

	auto iter = m_PartModels[eCharacterType].find(ePartType);

	if (iter == m_PartModels[eCharacterType].end())
		return nullptr;

	if (iter->second.size() <= iIndex)
		return nullptr;

	return iter->second[iIndex];
}


vector<class CModel*>* CPart_Manager::Get_PartModels(const CHARACTER_TYPE& eCharacterType, const PART_TYPE& ePartType)
{
	if (CHARACTER_TYPE::CHARACTER_END <= eCharacterType || PART_TYPE::PART_END < ePartType || eCharacterType < 0 || ePartType < 0)
		return nullptr;

	auto iter = m_PartModels[eCharacterType].find(ePartType);

	if (iter == m_PartModels[eCharacterType].end())
		return nullptr;

	return &(iter->second);
}

HRESULT CPart_Manager::Synchronize_PlayerAnimation_Init(const CHARACTER_TYPE& eCharacterType, class CModel* pDummyModel)
{
	if (0 > eCharacterType || CHARACTER_TYPE::CHARACTER_END <= eCharacterType || nullptr == pDummyModel)
		return E_FAIL;

	/* �÷��̾ ���� �ִ� �ִϸ��̼��� �÷��̾�Լ� ���� */
	/* �÷��̾�� ���� �ִϸ��̼��� �������� ���� ���� ���, �����鿡�� �ش� �ִϸ��̼��� �����Ѵ�. */
	for (auto& pPartModels : m_PartModels[eCharacterType]) /* �ش� ĳ���� Ÿ���� ��Ʈ ��ȸ*/
	{
		for (auto& pPartModel : pPartModels.second) /* ��Ʈ ���� ��ȸ */
		{
			for (size_t i = 0; i < pDummyModel->Get_Animations().size();)
			{
				/* ������ i��° �ִϸ��̼��� ��Ʈ���� ���� ���� ���� ���*/
				if (false == pPartModel->Has_Animation(pDummyModel->Get_Animations()[i]->Get_AnimationName()))
				{
					/* ���̿��� �ش� �ִϸ��̼��� �����Ѵ�. */
					pDummyModel->Delete_Animation(i);
				}
				else
				{
					i++;
				}
			}
		}
	}

	/* ������ ���� �ִ� �ִϸ��̼��� �������� ����*/
	/* �����鿡�� ���� �ִϸ��̼��� �÷��̾ ���� ���� ���, �÷��̾�Լ� �ش� �ִϸ��̼��� �����Ѵ�. */
	for (auto& pPartModels : m_PartModels[eCharacterType])
	{
		for (auto& pPartModel : pPartModels.second)
		{
			for (size_t i = 0; i < pPartModel->Get_Animations().size();) /* ��Ʈ�� ��� �ִϸ��̼� ��ȸ */
			{
				/* ��Ʈ ���� i��° �ִϸ��̼��� ���̰� ���� ���� ���� ���*/
				if (false == pDummyModel->Has_Animation(pPartModel->Get_Animations()[i]->Get_AnimationName()))
				{
					/* ��Ʈ �𵨿��� �ش� �ִϸ��̼��� ����*/
					pPartModel->Delete_Animation(i);
				}
				else
				{
					i++;
				}
			}
		}
	}

	/* ���� ���� */
	for (auto& pPartModels : m_PartModels[eCharacterType])
	{
		for (auto& pPartModel : pPartModels.second)
		{
			vector<class CAnimation*>& Animations = pPartModel->Get_Animations();

			sort(Animations.begin(), Animations.end(), [&](CAnimation* pSrcAnimation, CAnimation* pDestAnimation) {
				return pSrcAnimation->Get_AnimationName() < pDestAnimation->Get_AnimationName();
				});
		}
	}

	/* ���� ���� */
	vector<class CAnimation*>& Animations = pDummyModel->Get_Animations();
	sort(Animations.begin(), Animations.end(), [&](CAnimation* pSrcAnimation, CAnimation* pDestAnimation) {
		return pSrcAnimation->Get_AnimationName() < pDestAnimation->Get_AnimationName();
		});


	/* Apply */
	if (FAILED(Apply_PlayAnimation(eCharacterType)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPart_Manager::Synchronize_PlayerAnimation_Delete(const CHARACTER_TYPE& eCharacterType, const wstring strAnimName)
{
	if (0 > eCharacterType || CHARACTER_TYPE::CHARACTER_END <= eCharacterType)
		return E_FAIL;

	for (auto& pPartModels : m_PartModels[eCharacterType])
	{
		for (auto& pPartModel : pPartModels.second)
		{
			if (pPartModel->Has_Animation(strAnimName))
			{
				const _int iIndex = pPartModel->Get_AnimationIndex(strAnimName);
				if(0 <= iIndex)
					pPartModel->Delete_Animation(iIndex);
			}
		}
	}

	return S_OK;
}

HRESULT CPart_Manager::Synchronize_PlayerAnimation_ChangeName(const CHARACTER_TYPE& eCharacterType, const _uint& iAnimIndex, const wstring strName)
{
	if (0 > eCharacterType || CHARACTER_TYPE::CHARACTER_END <= eCharacterType)
		return E_FAIL;

	for (auto& pPartModels : m_PartModels[eCharacterType])
	{
		for (auto& pPartModel : pPartModels.second)
		{
			pPartModel->Get_Animation(iAnimIndex)->Set_AnimationName(strName);
		}
	}

	return S_OK;
}

HRESULT CPart_Manager::Synchronize_PlayerAnimation_Swap(const CHARACTER_TYPE& eCharacterType, const _uint& iAnimIndex, const _bool bUp)
{
	if (0 > eCharacterType || CHARACTER_TYPE::CHARACTER_END <= eCharacterType)
		return E_FAIL;

	for (auto& pPartModels : m_PartModels[eCharacterType])
	{
		for (auto& pPartModel : pPartModels.second)
		{
			if(bUp)
				pPartModel->Swap_Animation(iAnimIndex, iAnimIndex - 1);
			else
				pPartModel->Swap_Animation(iAnimIndex, iAnimIndex + 1);
		}
	}

	return S_OK;
}

HRESULT CPart_Manager::Synchronize_PlayerAnimation_Sort(const CHARACTER_TYPE& eCharacterType)
{
	if (0 > eCharacterType || CHARACTER_TYPE::CHARACTER_END <= eCharacterType)
		return E_FAIL;

	for (auto& pPartModels : m_PartModels[eCharacterType])
	{
		for (auto& pPartModel : pPartModels.second)
		{
			vector<class CAnimation*>& Animations = pPartModel->Get_Animations();

			sort(Animations.begin(), Animations.end(), [&](CAnimation* pSrcAnimation, CAnimation* pDestAnimation) {
				return pSrcAnimation->Get_AnimationName() < pDestAnimation->Get_AnimationName();
				});
		}
	}

	return S_OK;
}

HRESULT CPart_Manager::Apply_PlayAnimation(const CHARACTER_TYPE& eCharacterType)
{
	if (0 > eCharacterType || CHARACTER_TYPE::CHARACTER_END <= eCharacterType)
		return E_FAIL;

	for (auto& pPartModels : m_PartModels[eCharacterType])
	{
		for (auto& pPartModel : pPartModels.second)
		{
			wstring strStashPath = L"stach/";
			if (FAILED(GI->Export_Model_Data(pPartModel, strStashPath, pPartModel->Get_Name())))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CPart_Manager::Save_Parts(const wstring& strPath, const CHARACTER_TYPE& eCharacterType)
{
	if (0 > eCharacterType || CHARACTER_TYPE::CHARACTER_END <= eCharacterType)
		return E_FAIL;

	for (auto& pPartModels : m_PartModels[eCharacterType])
	{
		for (auto& pPartModel : pPartModels.second)
		{
			wstring wstrPrefix = L"..\\Bin";
			string strFindtarget = CUtils::ToString(pPartModel->Get_Name()) + ".mesh";
			filesystem::path prefix = "..\\Bin\\Export\\";
			filesystem::path result = CUtils::FindFile(wstrPrefix, strFindtarget);

			if (result.empty())
				continue;

			filesystem::path finalPath = CUtils::RemovePrefix(result, prefix);

			string strSavePath = finalPath.parent_path().string() + "\\";
			wstring strFileName = pPartModel->Get_Name();

			if (FAILED(GI->Export_Model_Data(pPartModel, CUtils::ToWString(strSavePath), strFileName)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CPart_Manager::Ready_PartModels()
{

	/* �δ����� ���� �����ϰ� ���⼭ Ŭ�� */

	/* SwordMan*/
	{
		/* Body */
		{	
			vector<CModel*> pSwordMan_Bodys;

			/* 01 */
			{
				CModel* pModel = dynamic_cast<CModel*>(GI->Clone_Component(LEVEL_STATIC, L"Prototype_Component_Model_SwordMan_Body_01"));
				if (nullptr == pModel)
				{
					MSG_BOX("Failed CPart_Manager::Ready_PartModels()");
					return E_FAIL;
				}
				pSwordMan_Bodys.push_back(pModel);
			}

			/* 02 */
			{
				CModel* pModel = dynamic_cast<CModel*>(GI->Clone_Component(LEVEL_STATIC, L"Prototype_Component_Model_SwordMan_Body_02"));
				if (nullptr == pModel)
				{
					MSG_BOX("Failed CPart_Manager::Ready_PartModels()");
					return E_FAIL;
				}
				pSwordMan_Bodys.push_back(pModel);
			}

			/* 02 */
			{
				CModel* pModel = dynamic_cast<CModel*>(GI->Clone_Component(LEVEL_STATIC, L"Prototype_Component_Model_SwordMan_Body_03"));
				if (nullptr == pModel)
				{
					MSG_BOX("Failed CPart_Manager::Ready_PartModels()");
					return E_FAIL;
				}
				pSwordMan_Bodys.push_back(pModel);
			}



			m_PartModels[CHARACTER_TYPE::SWORD_MAN].emplace(PART_TYPE::BODY, pSwordMan_Bodys);
		}

		/* Face */
		{			
			vector<CModel*> pSwordMan_Faces;

			/* 01 */
			CModel* pModel = dynamic_cast<CModel*>(GI->Clone_Component(LEVEL_STATIC, L"Prototype_Component_Model_SwordMan_Face_01"));
			if (nullptr == pModel)
			{
				MSG_BOX("Failed CPart_Manager::Ready_PartModels()");
				return E_FAIL;
			}
			pSwordMan_Faces.push_back(pModel);





			m_PartModels[CHARACTER_TYPE::SWORD_MAN].emplace(PART_TYPE::FACE, pSwordMan_Faces);
		}

		/* Hair */
		{
			vector<CModel*> pSwordMan_Hairs;

			/* 01 */
			CModel* pModel = dynamic_cast<CModel*>(GI->Clone_Component(LEVEL_STATIC, L"Prototype_Component_Model_SwordMan_Hair_01"));
			if (nullptr == pModel)
			{
				MSG_BOX("Failed CPart_Manager::Ready_PartModels()");
				return E_FAIL;
			}
			pSwordMan_Hairs.push_back(pModel);

			m_PartModels[CHARACTER_TYPE::SWORD_MAN].emplace(PART_TYPE::HAIR, pSwordMan_Hairs);
		}
	}

	return S_OK;
}

void CPart_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	for (_uint i = 0; i < CHARACTER_TYPE::CHARACTER_END; ++i)
	{
		for (auto& iter : m_PartModels[i])
		{
			for (auto& pModel : iter.second)
				Safe_Release(pModel);

			iter.second.clear();
		}
		m_PartModels[i].clear();
	}
}



