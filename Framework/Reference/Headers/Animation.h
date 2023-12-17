#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAnimation final : public CBase
{
public:
	
	/*typedef struct tagAnimEventDesc
	{
		ANIM_EVENT_TYPE eType;

	}ANIM_EVENT_DESC;*/

	typedef struct tagAnimSpeedDesc
	{
		_float fStartFrame = 0.f;
		_float fEndFrame = 0.f;

		_float fStartSpeed = 0.f;
		_float fEndSpeed = 0.f;

		tagAnimSpeedDesc() {};
		tagAnimSpeedDesc(const _float& _fPoint1, const _float& _fPoint2, const _float& _fValue1, const _float& _fValue2)
			: fStartFrame(_fPoint1), fEndFrame(_fPoint2), fStartSpeed(_fValue1), fEndSpeed(_fValue2) {}

	}ANIM_SPEED_DESC;


private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize_Prototype(aiAnimation* pAIAnimation); 
	HRESULT Initialize(class CModel* pModel); 

	/* ������ ���ǵ�� �̺�Ʈ üũ */
	void Update_Animation_Data(_float fTickPerSecond, const TWEEN_DESC& tDesc);
	void Clear_AnimationData();
public:

#pragma region Prop
	_float Get_Duration() { return m_fDuration; }

	void Set_AnimationName(const wstring& strName) { m_strName = strName; }
	const wstring& Get_AnimationName() { return m_strName; }

	void Set_Loop(_bool bLoop) { m_bLoop = bLoop; }
	_bool Is_Loop() { return m_bLoop; }

	_uint Get_MaxFrameCount() const { return _uint(m_fDuration) + 1; }
#pragma endregion

	
#pragma region Speed
	/* Tick Per Second */
	void Set_TickPerSecond(_float fTickPerSecond) { m_fTickPerSecond = fTickPerSecond; }
	_float Get_TickPerSecond() { return m_fTickPerSecond; }

	/* Origin Speed (�����Ӻ� �ǽð� �ӵ��� �������� ���� ������ �ӵ�)*/
	void Set_OriginSpeed(_float fSpeed) { m_fOriginSpeed = fSpeed; }
	_float Get_OriginSpeed() { return m_fOriginSpeed; }

	/* Live Speed (*�����Ӻ� �ǽð� �ӵ��� ������ ������ �ӵ�) */
	_float Get_LiveSpeed();

	/* Live Speed */
	void Add_SpeedDesc(ANIM_SPEED_DESC desc);
	void Delete_SpeedDesc(const _uint& iIndex);
	void Delete_All_SpeedDesc() { m_SpeedDescs.clear(); m_SpeedDescs.shrink_to_fit(); }
	void Change_SpeedDesc(const _uint& iIndex, const Vec4& vDesc);
	void Sort_SpeedDesces();
	vector<ANIM_SPEED_DESC> Get_SpeedDescs() const { return m_SpeedDescs; }

#pragma endregion


#pragma region Events



#pragma endregion


#pragma region FrameWork
public:
	class CChannel* Get_Channel(const wstring & strChannelName);
	const vector<class CChannel*>& Get_Channels() { return m_Channels; }

public:
	HRESULT Calculate_Animation(const _uint& iFrame);
	HRESULT Clear_Channels();
#pragma endregion

private:
	void Update_Animation_Speed(_float fTickPerSecond, const TWEEN_DESC& tDesc);
	void Update_Animation_Event(_float fTickPerSecond, const TWEEN_DESC& tDesc);

	const _float Calculate_LerpTime(const ANIM_SPEED_DESC tSpeedDesc, const TWEEN_DESC tTweenDesc, const _float fTickPerSecond);

private:
	wstring	m_strName;

	_bool m_bLoop = false;
	_float m_fDuration = 0.f; 

#pragma region Speed

private:
	_float m_fTickPerSecond = 0.f; 
	_float m_fOriginSpeed = 1.f;

	_uint	m_iCurSpeedDescIndex = 0; /* m_SpeedDescs�� ���� ���͸� �ε���*/
	_float	m_fCurSpeedDescEndFrame = 0; /* ���� �ֱٿ� ���� ������ m_SpeedDescs �ε����� ������ ������ */

	LERP_FLOAT_DESC m_tLiveSpeedDesc;
	vector<ANIM_SPEED_DESC> m_SpeedDescs;

#pragma endregion

#pragma region Events
private:

#pragma endregion



private: 
	_uint m_iNumChannels = 0;
	vector<class CChannel*>	m_Channels;
	vector<_uint> m_ChannelKeyFrames;
	vector<class CHierarchyNode*> m_HierarchyNodes;

public:
	static CAnimation* Create(aiAnimation* pAIAnimation);
	static CAnimation* Create_Bin();
	CAnimation* Clone(class CModel* pModel);
	virtual void Free() override;

public:
	friend class CModel_Manager;
};

END