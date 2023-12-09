#include "../Public/Timer_Manager.h"
#include "Timer.h"

IMPLEMENT_SINGLETON(CTimer_Manager)

CTimer_Manager::CTimer_Manager()
{
}

_float CTimer_Manager::Compute_TimeDelta(TIMER_TYPE eTimerType)
{
	CTimer*		pTimer = Find_Timer(eTimerType);

	if (nullptr == pTimer)
		return 0.f;

	/* Ÿ�ӵ�Ÿ�� ����ϰ� �����Ѵ�. */
	return pTimer->Compute_TimeDelta();	
}

HRESULT CTimer_Manager::Set_TimeScale(TIMER_TYPE eTimerType, _float fTimeScale)
{
	CTimer* pTimer = Find_Timer(eTimerType);

	if (nullptr == pTimer)
		return E_FAIL;

	pTimer->Set_TimeScale(fTimeScale);
	return S_OK;
}

_float CTimer_Manager::Get_TimeScale(TIMER_TYPE eTimerType)
{
	CTimer* pTimer = Find_Timer(eTimerType);

	if (nullptr == pTimer)
		return 0;

	return pTimer->Get_TimeScale();
}

_float CTimer_Manager::Get_TimeDelta(TIMER_TYPE eTimerType)
{
	CTimer* pTimer = Find_Timer(eTimerType);

	if (nullptr == pTimer)
		return 0.f;

	return pTimer->Get_TimeDelta();
}
HRESULT CTimer_Manager::Set_Slow(TIMER_TYPE eTimerType, _float fSlowTime, _float fTimeScale, _bool bForce)
{
	CTimer* pTimer = Find_Timer(eTimerType);

	if (nullptr == pTimer)
		return E_FAIL;


	return pTimer->Set_Slow(fSlowTime, fTimeScale, bForce);
}


HRESULT CTimer_Manager::Add_Timer(TIMER_TYPE eTimerType)
{
	/* map�� �ߺ��� Ű�� ������� �ʴ���.*/
	CTimer*		pTimer = Find_Timer(eTimerType);

	/* �߰��ϰ����ϴ� strTimerTag�� �ش��ϴ� Pair�����Ͱ� �̹� �߰��Ǿ��־���. */
	if (nullptr != pTimer)
		return E_FAIL;

	m_Timers.insert({ eTimerType, CTimer::Create() });

	return S_OK;
}




CTimer * CTimer_Manager::Find_Timer(TIMER_TYPE eTimerType)
{
	/* �ʿ��� �������ִ� Find�Լ��� ����Ž���� �����Ѵ�. */
	auto	iter = m_Timers.find(eTimerType);

	if (iter == m_Timers.end())
		return nullptr;

	return iter->second;
}

void CTimer_Manager::Free(void)
{
	__super::Free();

	for (auto& Pair : m_Timers)
	{
		Safe_Release(Pair.second);
	}
	m_Timers.clear();
}