#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Stellia_Skill_TripleLaser final : public CVfx
{
private:
	enum TYPE {
		TYPE_ET1_D_WARNING_CIRCLE,

		// �߰� ��� ���� // ���� �ѵι� ���� 13 - a
		TYPE_ET2_E_MARBLE_MU,
		// ���� �ϴ� ���� // 38 - a
		TYPE_ET2_E_MARBLE_LD,
		// ���� �ϴ� ���� // 59 - a
		TYPE_ET2_E_MARBLE_RD,

		// ���ÿ� ������ �������� �� �߻� -> ���� �� ������ �߻� / ���� // 90

		// ��¦��

		TYPE_END
	};

	// ������ ũ�� ���� ����� �ֺ��� �ƿ�� �����. ������ ���� �۾�����. ������ ���� ġ���� �귯��
	//  ������ �� �Ȱ��� ũ�Ⱑ �Ǹ� ���� �Ʒ��� 3���� ������ �߻簡 �ȴ�. ���ڰ� �޽��ε�.
	// �������� ���ʹ������� ���ư��鼭 ����� ���̽��� �߹��뤷 �����. �������� ����� ��¦�� �ֳ�.

protected:
	CVfx_Stellia_Skill_TripleLaser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Stellia_Skill_TripleLaser(const CVfx_Stellia_Skill_TripleLaser& rhs);
	virtual ~CVfx_Stellia_Skill_TripleLaser() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

private:
	class CDecal* m_pWarningDecal = nullptr;

public:
	static CVfx_Stellia_Skill_TripleLaser* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END