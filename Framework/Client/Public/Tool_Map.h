#pragma once
#include "Tool.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CTool_Map final : public CTool
{
private:
	CTool_Map(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Map() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;

private:
	void AddMapObject(LEVELID iLevelID, LAYER_TYPE iLayerType);
	void DeleteObject(LEVELID iLevelID, LAYER_TYPE iLayerType);
	void BatchObject(LEVELID iLevelID, LAYER_TYPE iLayerType);
	void Picking();

private:
	HRESULT Save_Map_Data(const wstring& strMapFileName);
	HRESULT Load_Map_Data(const wstring& strMapFileName);
private:
	// ������Ʈ �������� 
	_bool m_bAddObject = false;

	// �� ��Ʈ�� �������
	_int m_iControlState = 0;

	// ������ ������Ʈ
	CGameObject* m_pSelectObj = nullptr;
public:
	static CTool_Map* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END
