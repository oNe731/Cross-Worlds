#pragma once

#include "Client_Defines.h"
#include "Vfx.h"

BEGIN(Engine)
END

BEGIN(Client)

class CVfx_Engineer_Skill_ExplosionShot final : public CVfx
{
private:
	enum TYPE {
		// ���
		TYPE_ET1_D_RECT,
		// ------------------------------------------
		// �Ѿ� �߻� -> �Ѿ� �ڿ� Ʈ���� �� ��¦�� ���±濡 � �ڿ� �긲
		// ��½�ϴ� ��ƼŬ
		TYPE_ET1_E_CIRCLELINE,// ���� Ŀ���� ������ ����Ʈ -> �� ȸ������ ����
		TYPE_ET1_E_CIRCLES,   // ���� �� ��ƼŬ�� �
		TYPE_ET1_P_FIRE,      // �Ҳ� ��ƼŬ
		TYPE_ET1_P_SMOKE,     // ���� ��ƼŬ
		// ------------------------------------------
		TYPE_ET2_D_CIRCLE,    // Ǫ�� �� ��Į
		TYPE_ET2_P_DOME,      // ���� �� ����Ʈ
		TYPE_ET2_P_FIRE,      // ���� �Ҳ� ��ƼŬ
		TYPE_ET2_E_CIRCLELINE,// ȸ���� ����Ʈ
		TYPE_ET2_P_CIRCLES,   // ���� ��¦�� ��ƼŬ
		TYPE_ET2_P_SMOKE,     // ���� ��ƼŬ

		TYPE_END
	};

protected:
	CVfx_Engineer_Skill_ExplosionShot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVfx_Engineer_Skill_ExplosionShot(const CVfx_Engineer_Skill_ExplosionShot& rhs);
	virtual ~CVfx_Engineer_Skill_ExplosionShot() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CVfx_Engineer_Skill_ExplosionShot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END