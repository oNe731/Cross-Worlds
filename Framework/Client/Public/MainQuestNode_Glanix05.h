#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_Glanix05 final : public CQuestNode_Base
{
private:
	CMainQuestNode_Glanix05();
	virtual ~CMainQuestNode_Glanix05() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pJackson = nullptr;
	CGameObject* m_pKuu = nullptr;

	_float m_fTalkChangeTime = 3.f;
	_float m_fTime = 0.f;

public:
	static CMainQuestNode_Glanix05* Create();
	virtual void Free() override;
};

