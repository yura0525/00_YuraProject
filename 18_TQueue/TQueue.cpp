#include "TQueue.h"
#include <iostream>
using namespace std;

int TQueue::Get()
{
	if (m_iFront== m_iNear)
	{
		cout << "TQueue Underflow" << endl;
		return -1;
	}
	int iValue = m_iQueueArray[m_iFront];
	m_iQueueArray[m_iFront] = -1;
	m_iFront = (++m_iFront) % g_iMaxQueue;
	return iValue;
}
int TQueue::Put(int iData)
{
	//방하나는 비워야한다. g_iMaxQueue == 5이면 4개까지만 들어간다.
	if (((m_iNear+1) % g_iMaxQueue) == m_iFront)
	{
		cout << "TQueue Overflow" << endl;
		return -1;
	}
	m_iQueueArray[m_iNear] = iData;
	m_iNear = (++m_iNear) % g_iMaxQueue;
	return m_iNear;
}
void TQueue::Show()
{

}

TQueue::TQueue() : m_iFront(0), m_iNear(0)
{
}


TQueue::~TQueue()
{
}

