#include <winsock2.h>
#include <ws2tcpip.h>	//InetNtop(), inet_ntop();
#include <iostream>
#include <list>
#include <map>

#pragma comment(lib, "ws2_32")

#define MAX_WORK_THREAD 4
#define RECV 1000
#define SEND 2000

struct OVERLAPPED_EX : OVERLAPPED
{
	DWORD flag;
};

struct TUser
{
	SOCKET sock;
	WSABUF buffer;
	char buf[2048];
	OVERLAPPED_EX ov;
};
TUser user;

HANDLE m_hIOCP;
HANDLE m_hWorkThread[MAX_WORK_THREAD];

bool BeginWinSock()
{
	WSADATA wsa;

	int iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (iRet != 0)	return false;

	return true;
}
bool EndWinSock()
{
	int iRet = WSACleanup();
	if (iRet != 0)	return false;

	return true;
}

DWORD WINAPI WorkerThread(LPVOID param)
{
	LPOVERLAPPED ov;
	DWORD bytesTransfer;
	ULONG_PTR keyValue;

	while (1)
	{
		//�񵿱� ������� �۾������ Ȯ���ϴ� �Լ�.
		//::���� �����Լ�. 
		//::����� ���� �Լ��� �򰥸��� ����� ���Լ��� ���� Ŭ������ �ƴ϶� ������ �ִٰ� �˷��ִ� �Լ�.
		//IOCP QUEUE
		bool bRet = ::GetQueuedCompletionStatus(m_hIOCP,
			&bytesTransfer, &keyValue,
			&ov, 1);

		TUser* pUser = (TUser*)keyValue;
		OVERLAPPED_EX* ovex = (OVERLAPPED_EX*)&ov;

		if (bRet == TRUE)
		{
			if (ovex->flag == RECV)
			{
				DWORD trans = bytesTransfer;
				ovex->flag = SEND;

				int iRet = WSASend(pUser->sock, &(pUser->buffer), 1, &trans, 0,
					(LPOVERLAPPED)&(pUser->ov), NULL);
			}
			else
			{
				pUser->buffer.buf = pUser->buf;
				pUser->buffer.len = 2048;

				DWORD trans = bytesTransfer;
				ovex->flag = SEND;

				int iRet = WSASend(pUser->sock, &(pUser->buffer), 1, &trans, 0,
					(LPOVERLAPPED)&(pUser->ov), NULL);
				/*if (iRet == SOCKET_ERROR)
				{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
				return false;
				}
				}*/
			}
		}
	}
	return 0;
}
int main()
{
	if (BeginWinSock() == false)
	{
		return -1;
	}

	// listen socket
	SOCKET listenSock;
	listenSock = socket(AF_INET, SOCK_STREAM, 0);

	// ip + port -> bind -> � ü�� ����.
	// port�� ����ϰ� �ִ� ���μ���(���������� �����ϴ� �뵵�� �Ѵ�.)���� �˷��ش�.
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));

	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	//sa.sin_addr.s_addr = inet_addr("192.168.0.27");
	//ip�� �������� ��쿡(����, ������) �������� �ƹ��ŷ� �� �޴´ٴ� ��.
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(listenSock, (sockaddr*)&sa, sizeof(sa));

	// listen -> ���.
	listen(listenSock, SOMAXCONN);	//->����

	//������4���� ���.
	DWORD id;
	for (int i = 0; i < MAX_WORK_THREAD; i++)
	{
		m_hWorkThread[i] = ::CreateThread(0, 0, WorkerThread, 0, 0, &id);
	}

	//����ڸ� ���.
	m_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);

	while (1)
	{
		SOCKET sock = accept(listenSock, (SOCKADDR*)&addr, &addrlen);
		user.sock = sock;

		//����ڿ��� Read���Ͽ����� �д� ����� �Ϸ�Ǹ�, �� ����� �Ϸ�Űkey( ex)1111 )�� �������� �˷����.
		::CreateIoCompletionPort((HANDLE)sock, m_hIOCP, (ULONG_PTR)&user, 0);
		
		DWORD trans, flag = 0;
		user.buffer.buf = user.buf;
		user.buffer.len = 2048;
		user.ov.flag = RECV;

		int iRet = WSARecv(sock, &user.buffer, 1, &trans, &flag, (LPOVERLAPPED)&user.ov, NULL);
		if (iRet == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				return false;
			}
		}
	}

	EndWinSock();
}

