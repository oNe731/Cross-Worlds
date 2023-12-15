#pragma once
#include "Tool.h"

BEGIN(Client)

class CTool_Model final : public CTool
{
private:
	CTool_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Model() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;

private:
	void Tick_Model(_float fTimeDelta);
	void Tick_Animation(_float fTimeDelta);
	void Tick_Socket(_float fTimeDelta);
	void Tick_Event(_float fTimeDelta);
	void Tick_Costume(_float fTimeDelta);

private:
	const _bool Is_Exception();

private:
	HRESULT Ready_WeaponPrototypes();


private:
	void Reset_Transform();

private:
	wstring m_strFilePath = L"";
	wstring m_strFileName = L"";

	class CDummy* m_pDummy = nullptr;

private:

	/* Socket */

	/* ���̾Ű ��� */
	_int m_iCurBoneIndex = 0;
	
	/* ������Ÿ�� ����*/
	vector<class CPart*> m_WeaponPrototypes;
	_uint m_iCurWeaponIndex = 0;

	/* ���� ���� ��� �����̳� (���ε���, Ʈ��������) */
	map<_uint, vector<ANIM_TRANSFORM_CACHE>> m_CalculatedSockets;


public:
	static CTool_Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END