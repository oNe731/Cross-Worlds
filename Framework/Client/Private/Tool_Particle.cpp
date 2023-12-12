#include "stdafx.h"
#include "Tool_Particle.h"
#include "imgui.h"

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

	// Ʈ������ // 
	   // ������
	   // �����̼�
	   // ������

	// ��ƼŬ �ý��� //
	   // Duration          / ���ӽð�
	   // Looping           / �ݺ�����
	   // Start Delay       / ���� ���� ���� �ð�
	   // Start Lifetime    / ���� �ֱ� (���� ������°�)
	   // Start Speed       / ���� ���� �ӵ� (���/ ����)
	   // Start Size        / ���� ũ��
	   // Start Rotation    / ���� ����
	   // Start Color       / ���� ����
	   // Play On Awake     / �� ���� �� ��� ����
	   // Emitter Velocity  / ����ӵ�
	   // Max Particles     / �ִ� ���ڼ�

	   // Gravity Modifier  / �߷� ȿ��
	   // Auto Random Seed  / ����� ������ �������� ǥ��
	   // Stop Action       / ��� ��ƼŬ�� �Ϸ�Ǿ� �ý����� �����Ǿ��� ��
	   // Culling Mode      / ȭ���� ����� �Ͻ������� ���ΰ�
	   // Ring Buffer Mode  / max particle ���� ������ ������ ��ƼŬ�� ��� Ȱ��ȭ/ �ƽ��� ���� �� ������ ��ƼŬ ��Ȱ��


	ImGui::End();
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
