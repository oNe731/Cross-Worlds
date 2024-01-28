#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_Invasion04 final : public CQuestNode_Base
{
private:
	CMainQuestNode_Invasion04();
	virtual ~CMainQuestNode_Invasion04() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	void Delete_Npc();
	HRESULT Load_Npc();

private:
	CGameObject* m_pRuslan = nullptr;

public:
	static CMainQuestNode_Invasion04* Create();
	virtual void Free() override;
};

