#pragma once
#include <iostream>
const int g_iMaxQueue = 5;

class TQueue
{
public:
	int m_iQueueArray[g_iMaxQueue];
	int m_iFront;
	int m_iNear;
public:
	int Get();
	int Put(int iData);
	void Show();

public:
	TQueue();
	virtual ~TQueue();
};

