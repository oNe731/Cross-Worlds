#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_Wanted03_2 final : public CQuestNode_Base
{
private:
	CSubQuestNode_Wanted03_2();
	virtual ~CSubQuestNode_Wanted03_2() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pCriminal = nullptr;
	CGameObject* m_pKuu = nullptr;

	_bool m_bIsSkipEvent = false;

	_float m_fAccSkipTime = 0.f;
	_float m_fSkipTime = 0.f;

private:
	Vec4 m_vCirminalOriginLookAt = {};
public:
	static CSubQuestNode_Wanted03_2* Create();
	virtual void Free() override;
};

