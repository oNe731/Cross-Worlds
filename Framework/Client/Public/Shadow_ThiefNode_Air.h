#pragma once

#include "Shadow_ThiefNode_Base.h"

BEGIN(Client)

class CShadow_ThiefNode_Air final : public CShadow_ThiefNode_Base
{
private:
	CShadow_ThiefNode_Air();
	virtual ~CShadow_ThiefNode_Air() = default;

public:
	virtual HRESULT Initialize_Prototype(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

private:
	_float m_fTime = 0.f;
	_float m_fAirTime = 0.f;

	_bool m_bIsStand = false;


public:
	static CShadow_ThiefNode_Air* Create(CMonsterBT::BT_MONSTERDESC* pDesc, CMonsterBT* pBT);
	virtual void Free() override;
};

END

