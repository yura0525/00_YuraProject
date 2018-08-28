#include "TStandAction.h"


bool TStandAction::Process(TObject* pTarget)
{
	RECT rt;
	rt.left = m_pOwner->m_pos.x - m_pOwner->m_fAttackRadius;
	rt.top = m_pOwner->m_pos.y - m_pOwner->m_fAttackRadius;
	rt.right = m_pOwner->m_pos.x + m_pOwner->m_fAttackRadius;
	rt.bottom = m_pOwner->m_pos.y + m_pOwner->m_fAttackRadius;

	m_pOwner->m_rtCollision.left = rt.left;
	m_pOwner->m_rtCollision.top = rt.left;
	m_pOwner->m_rtCollision.left = rt.left;
	m_pOwner->m_rtCollision.left = rt.left;

	if ((rand() % 100) == 5)
	{
		m_pOwner->SetTransition(EVENT_PATROL);
	}

	if (TCollision::RectInRect(rt, pTarget->m_rtCollision) )
	{
		m_pOwner->SetTransition(EVENT_FINDTARGET);
	}
	else
	{
		if (m_pOwner->m_dwCurrentState == STATE_MOVE)
		{
			m_pOwner->SetTransition(EVENT_LOSTTARGET);
		}
	}
	
	return true;
}

TStandAction::TStandAction(TEnemy * pOwner) : TEnemyState(pOwner)
{
}


TStandAction::~TStandAction()
{
}
