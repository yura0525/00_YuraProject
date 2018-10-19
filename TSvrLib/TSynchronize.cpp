#include "stdafx.h"
#include "TSynchronize.h"


TSynchronize::TSynchronize(TSvrObject* pObj)
{
	m_pSerObj = pObj;
	if (m_pSerObj != NULL)
	{
		EnterCriticalSection(&m_pSerObj->m_cs);
	}
}


TSynchronize::~TSynchronize()
{
	if (m_pSerObj != NULL)
	{
		LeaveCriticalSection(&m_pSerObj->m_cs);
	}
}
