#pragma once
#include "TThread.h"
#include "TNetwork.h"

class TAccept : public TThread
{
public:
	TNetwork m_network;
public:
	bool Set(int iPort, const char* address = NULL);
	virtual void Run();
public:
	TAccept();
	virtual ~TAccept();
};

