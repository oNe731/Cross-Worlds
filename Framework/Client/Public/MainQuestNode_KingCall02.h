#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_KingCall02 final : public CQuestNode_Base
{
private:
	CMainQuestNode_KingCall02();
	virtual ~CMainQuestNode_KingCall02() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pSanson = nullptr;
	CGameObject* m_pKuu = nullptr;

public:
	static CMainQuestNode_KingCall02* Create();
	virtual void Free() override;
};

