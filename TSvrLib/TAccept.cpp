#include "stdafx.h"
#include "TAccept.h"
#include "TSynchronize.h"

bool TAccept::Set(int iPort, const char* address)
{
	return m_network.Set(iPort, address);
}
void TAccept::Run()
{
	while (1)
	{
		SOCKADDR_IN clientaddr;
		int addlen = sizeof(clientaddr);
		SOCKET client = accept(m_network.m_Sock, (sockaddr*)&clientaddr, &addlen);

		if (client == INVALID_SOCKET)
		{
			continue;
		}
		{
			TSynchronize sync(this);
		}
		Sleep(1000);
	}
}
TAccept::TAccept()
{
}


TAccept::~TAccept()
{
}
