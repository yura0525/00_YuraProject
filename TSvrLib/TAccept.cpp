#include "stdafx.h"
#include "TAccept.h"
#include "TSynchronize.h"
#include "TSvrIOCP.h"
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
		SOCKET clientSocket = accept(m_network.m_ListenSock, (sockaddr*)&clientaddr, &addlen);

		if (clientSocket == INVALID_SOCKET)
		{
			continue;
		}
		
		AddUser(clientSocket, clientaddr);
		
		Sleep(1);
	}
}
XUser* TAccept::AddUser(SOCKET clientSocket, SOCKADDR_IN clientAddr)
{
	XUser* pUser =  I_User.AddUser(clientSocket, clientAddr);
	if (pUser != NULL)
	{
		//io -> model
		I_IOCP.Add((HANDLE)clientSocket, (ULONG_PTR)pUser);
		pUser->RecvData();
		
	}
	return pUser;
}
TAccept::TAccept()
{
}


TAccept::~TAccept()
{
}
