#include "stdafx.h"
#include "Tool_Particle.h"
#include "imgui.h"

#include "GameInstance.h"
#include "GameObject.h"

CTool_Particle::CTool_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTool(pDevice, pContext)
{
}

HRESULT CTool_Particle::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CTool_Particle::Tick(_float fTimeDelta)
{
	ImGui::Begin("Particle_Tool");

	// ����
	if (ImGui::Button("Create"))
	{
		if (m_pParticle == nullptr)
		{
			// ����
			m_pParticle = GI->Clone_GameObject(TEXT("Prototype_GameObject_Particle"), LAYER_TYPE::LAYER_EFFECT);
			GI->Add_GameObject(LEVEL_TOOL, LAYER_TYPE::LAYER_EFFECT, m_pParticle);

			// �⺻ �� �ʱ�ȭ
			CTransform* pTransform = m_pParticle->Get_Component<CTransform>(L"Com_Transform");
			_vector vPosition = pTransform->Get_State(CTransform::STATE_POSITION);
			m_fPosition[0] = XMVectorGetX(vPosition);
			m_fPosition[1] = XMVectorGetY(vPosition);
			m_fPosition[2] = XMVectorGetZ(vPosition);

			_vector vRotation = pTransform->Get_WorldRotation();
			m_fRotation[0] = XMVectorGetX(vRotation);
			m_fRotation[1] = XMVectorGetY(vRotation);
			m_fRotation[2] = XMVectorGetZ(vRotation);

			_float3 fScale = pTransform->Get_Scale();
			m_fScale[0] = fScale.x;
			m_fScale[1] = fScale.y;
			m_fScale[2] = fScale.z;

			m_tParticleInfo = static_cast<CParticle*>(m_pParticle)->Get_ParticleDesc();

			m_fParticleScale[0] = m_tParticleInfo.fScale.x;
			m_fParticleScale[1] = m_tParticleInfo.fScale.y;

			m_fParticleRange[0] = m_tParticleInfo.fRange.x;
			m_fParticleRange[1] = m_tParticleInfo.fRange.y;
			m_fParticleRange[2] = m_tParticleInfo.fRange.z;

			m_fParticleLifeTime[0] = m_tParticleInfo.fLifeTime.x;
			m_fParticleLifeTime[1] = m_tParticleInfo.fLifeTime.y;

			m_fParticleSpeed[0] = m_tParticleInfo.fSpeed.x;
			m_fParticleSpeed[1] = m_tParticleInfo.fSpeed.y;

			m_fParticleBoxMin[0] = m_tParticleInfo.fBoxMin.x;
			m_fParticleBoxMin[1] = m_tParticleInfo.fBoxMin.y;
			m_fParticleBoxMin[2] = m_tParticleInfo.fBoxMin.z;

			m_fParticleBoxMax[0] = m_tParticleInfo.fBoxMax.x;
			m_fParticleBoxMax[1] = m_tParticleInfo.fBoxMax.y;
			m_fParticleBoxMax[2] = m_tParticleInfo.fBoxMax.z;

			m_fParticleVelocityMin[0] = m_tParticleInfo.vVelocityMin.x;
			m_fParticleVelocityMin[1] = m_tParticleInfo.vVelocityMin.y;
			m_fParticleVelocityMin[2] = m_tParticleInfo.vVelocityMin.z;

			m_fParticleVelocityMax[0] = m_tParticleInfo.vVelocityMax.x;
			m_fParticleVelocityMax[1] = m_tParticleInfo.vVelocityMax.y;
			m_fParticleVelocityMax[2] = m_tParticleInfo.vVelocityMax.z;

			m_fParticleAnimationSpeed[0] = m_tParticleInfo.fAnimationSpeed.x;
			m_fParticleAnimationSpeed[1] = m_tParticleInfo.fAnimationSpeed.y;
		}
	}
	ImGui::SameLine();
	// ����
	if (ImGui::Button("Delete"))
	{
		if (m_pParticle != nullptr)
		{
			m_pParticle->Set_Dead(true);
			m_pParticle = nullptr;

			// �ʱ�ȭ
			m_fPosition[0] = 0.f;
			m_fPosition[1] = 0.f;
			m_fPosition[2] = 0.f;

			m_fRotation[0] = 0.f;
			m_fRotation[1] = 0.f;
			m_fRotation[2] = 0.f;

			m_fScale[0] = 0.f;
			m_fScale[1] = 0.f;
			m_fScale[2] = 0.f;
		}
	}

	// Ʈ������ ����
	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::Text("Position");
		ImGui::InputFloat3("##Position", m_fPosition);

		ImGui::Text("Rotation");
		ImGui::InputFloat3("##Rotation", m_fRotation);

		ImGui::Text("Scale");
		ImGui::InputFloat3("##Scale", m_fScale);

		if (ImGui::Button("Select_Transform"))
		{
			if (m_pParticle != nullptr)
			{
				CTransform* pTransform = m_pParticle->Get_Component<CTransform>(L"Com_Transform");

				pTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fPosition[0], m_fPosition[1], m_fPosition[2], 1.f));
				pTransform->FixRotation(m_fRotation[0], m_fRotation[1], m_fRotation[2]);
				pTransform->Set_Scale(_float3(m_fScale[0], m_fScale[1], m_fScale[2]));
			}
		}
	}

	// ��ƼŬ ����
	if (ImGui::CollapsingHeader("ParticleSystem"))
	{
		// �ݺ� ����
		ImGui::Checkbox("LoopParticles", &m_tParticleInfo.bLoop);
		ImGui::NewLine();

		// ��ƼŬ ����
		ImGui::Text("MaxParticles");
		ImGui::InputInt("##MaxParticles", &(_int)m_tParticleInfo.iNumEffectCount);
		ImGui::NewLine();

		// ���� ����
		ImGui::Text("RangeParticles");
		ImGui::InputFloat3("##RangeParticles", m_fParticleRange);
		ImGui::NewLine();

		// ���� ũ��
		ImGui::Checkbox("SameRate", &m_tParticleInfo.bSameRate);
		ImGui::Text("SizeParticles");
		ImGui::InputFloat2("##SizeParticles", m_fParticleScale);
		ImGui::NewLine();

		// ���� �ð�
		if (ImGui::CollapsingHeader("ParticleTime"))
		{
			ImGui::Text("LifetimeParticles");
			ImGui::InputFloat2("##LifetimeParticles", m_fParticleLifeTime);
			ImGui::NewLine();
		}

		// �̵�(��)
		if (ImGui::CollapsingHeader("ParticleVelocity"))
		{
			// �̵� �ӵ�
			ImGui::Text("SpeedParticles");
			ImGui::InputFloat2("##SpeedParticles", m_fParticleSpeed);
			ImGui::NewLine();

			// ������
			ImGui::Text("VelocityMinParticles");
			ImGui::InputFloat3("##VelocityMinParticles", m_fParticleVelocityMin);
			ImGui::Text("VelocityMaxParticles");
			ImGui::InputFloat3("##VelocityMaxParticles", m_fParticleVelocityMax);
			ImGui::NewLine();
		}

		// �ڽ� ����
		if (ImGui::CollapsingHeader("ParticleBox"))
		{
			ImGui::Checkbox("UseBox", &m_tParticleInfo.bUseBox);
			if (m_tParticleInfo.bUseBox)
			{
				ImGui::Text("BoxMinParticles");
				ImGui::InputFloat3("##BoxMinParticles", m_fParticleBoxMin);
				ImGui::Text("BoxMaxParticles");
				ImGui::InputFloat3("##BoxMaxParticles", m_fParticleBoxMax);
				ImGui::NewLine();
			}
		}

		// ��ƼŬ ����
		if (ImGui::CollapsingHeader("ParticleColor"))
		{
			ImGui::Checkbox("RandomColor", &m_tParticleInfo.bRandomColor);
			if (!m_tParticleInfo.bRandomColor)
			{
				ImGui::Text("Diffuse Color");
				ImGui::ColorEdit4("##DiffuseColor", (float*)&m_tParticleInfo.vDiffuseColor, ImGuiColorEditFlags_Float);
			}
		}

		// �ؽ�ó ����
		if (ImGui::CollapsingHeader("ParticleTexture"))
		{
			// ��ǻ�� �ؽ�ó ����
			ImGui::Text("DiffuseTextureName :");
			ImGui::SameLine();
			ImGui::InputText("DiffuseTextureName", m_cDiffuseTextureName, IM_ARRAYSIZE(m_cDiffuseTextureName));

			// ���� �ؽ�ó ����
			ImGui::Text("AlphaTextureName :");
			ImGui::SameLine();
			ImGui::InputText("AlphaTextureName", m_cAlphaTextureName, IM_ARRAYSIZE(m_cAlphaTextureName));


			// ��ǻ�� �ؽ�ó ���� �ؽ�ó �ε���
			ImGui::Checkbox("RandomStartTextureIndex", &m_tParticleInfo.bRandomStartIndex);
			if (!m_tParticleInfo.bRandomStartIndex)
			{
				ImGui::Text("StartTextureIndex");
				ImGui::InputFloat("##StartTextureIndex", &m_tParticleInfo.fDiffuseTextureIndex);
			}
			ImGui::NewLine();
		}

		// �ؽ�ó �ִϸ��̼�
		if (ImGui::CollapsingHeader("ParticleAnimation"))
		{
			ImGui::Checkbox("UseAnimation", &m_tParticleInfo.bAnimation);
			if (m_tParticleInfo.bAnimation)
			{
				ImGui::Checkbox("LoopAnimation", &m_tParticleInfo.bAnimationLoop);

				ImGui::Text("SpeedAnimation");
				ImGui::InputFloat2("##SpeedAnimation", m_fParticleAnimationSpeed);
			}
		}

	    // Gravity Modifier  / �߷� ȿ��

		if (ImGui::Button("Select_ParticleSystem"))
		{
			if (m_pParticle != nullptr)
			{
				m_tParticleInfo.fScale = _float2(m_fParticleScale[0], m_fParticleScale[1]);
				m_tParticleInfo.fRange = _float3(m_fParticleRange[0], m_fParticleRange[1], m_fParticleRange[2]);
				m_tParticleInfo.fLifeTime = _float2(m_fParticleLifeTime[0], m_fParticleLifeTime[1]);
				m_tParticleInfo.fSpeed = _float2(m_fParticleSpeed[0], m_fParticleSpeed[1]);
				m_tParticleInfo.fBoxMin = _float3(m_fParticleBoxMin[0], m_fParticleBoxMin[1], m_fParticleBoxMin[2]);
				m_tParticleInfo.fBoxMax = _float3(m_fParticleBoxMax[0], m_fParticleBoxMax[1], m_fParticleBoxMax[2]);
				m_tParticleInfo.vVelocityMin = _float3(m_fParticleVelocityMin[0], m_fParticleVelocityMin[1], m_fParticleVelocityMin[2]);
				m_tParticleInfo.vVelocityMax = _float3(m_fParticleVelocityMax[0], m_fParticleVelocityMax[1], m_fParticleVelocityMax[2]);
				m_tParticleInfo.fAnimationSpeed = _float2(m_fParticleAnimationSpeed[0], m_fParticleAnimationSpeed[1]);

				wstring strDiffuseTextureName(m_cDiffuseTextureName, m_cDiffuseTextureName + strlen(m_cDiffuseTextureName));
				m_tParticleInfo.strDiffuseTetextureName = strDiffuseTextureName;
				wstring strAlphaTextureName(m_cAlphaTextureName, m_cAlphaTextureName + strlen(m_cAlphaTextureName));
				m_tParticleInfo.strAlphaTexturName = strAlphaTextureName;

				static_cast<CParticle*>(m_pParticle)->Set_ParticleDesc(m_tParticleInfo);
			}
		}

		// �����ϱ�/ �ҷ�����
		if (ImGui::Button("Save"))
			Save_Particle();
		ImGui::SameLine();
		if (ImGui::Button("Load"))
			Load_Particle();
	}

	ImGui::End();
}

void CTool_Particle::Save_Particle()
{
}

void CTool_Particle::Load_Particle()
{
}

CTool_Particle* CTool_Particle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Particle* pInstance = new CTool_Particle(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed_Create : CTool_Particle");
		return nullptr;
	}
	return pInstance;
}

void CTool_Particle::Free()
{
	__super::Free();
}
