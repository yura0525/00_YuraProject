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
		//비동기 입출력의 작업결과를 확인하는 함수.
		//::범위 지정함수. 
		//::사용자 정의 함수와 헷갈리지 말라고 이함수는 여기 클래스가 아니라 상위에 있다고 알려주는 함수.
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

	// ip + port -> bind -> 운영 체제 보고.
	// port를 사용하고 있는 프로세스(실행파일을 구분하는 용도로 한다.)에게 알려준다.
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));

	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	//sa.sin_addr.s_addr = inet_addr("192.168.0.27");
	//ip가 여러개인 경우에(랜선, 무선랜) 여러개중 아무거로 다 받는다는 뜻.
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(listenSock, (sockaddr*)&sa, sizeof(sa));

	// listen -> 듣다.
	listen(listenSock, SOMAXCONN);	//->개통

	//스레드4개를 고용.
	DWORD id;
	for (int i = 0; i < MAX_WORK_THREAD; i++)
	{
		m_hWorkThread[i] = ::CreateThread(0, 0, WorkerThread, 0, 0, &id);
	}

	//기술자를 고용.
	m_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);

	while (1)
	{
		SOCKET sock = accept(listenSock, (SOCKADDR*)&addr, &addrlen);
		user.sock = sock;

		//기술자에게 Read파일에대한 읽는 결과가 완료되면, 그 결과를 완료키key( ex)1111 )의 내용으로 알려줘라.
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

