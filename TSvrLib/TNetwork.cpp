#include "stdafx.h"
#include "TNetwork.h"

bool TNetwork::Set(int iPort, const char* address)
{
	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);

	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (address == NULL)
	{
		if (m_Sock == INVALID_SOCKET)
		{
			return false;
		}

		SOCKADDR_IN addr;
		ZeroMemory(&addr, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(10000);

		//¹ÙÀÎµù
		int ret = bind(m_Sock, (sockaddr*)&addr, sizeof(addr));
		if (ret == SOCKET_ERROR)
		{
			return false;
		}

		ret = listen(m_Sock, SOMAXCONN);
		if (ret == SOCKET_ERROR)
		{
			return false;
		}
	}
	else
	{

	}
	return true;
}

bool TNetwork::Release()
{
	return true;
}
TNetwork::TNetwork()
{
}


TNetwork::~TNetwork()
{
}
