#include <winsock2.h>
#include <ws2tcpip.h>	//InetNtop(), inet_ntop();
#include <iostream>
#include <map>
#pragma comment(lib, "ws2_32")
using namespace std;

bool BeginWinSock();
bool EndWinSock();

struct TUser
{
	SOCKET sock;
	SOCKADDR_IN clientAddr;
};
void tMSG(TUser* pUser, const char* msg)
{
	printf("\n ip = %s, msg = %s",
		inet_ntoa(pUser->clientAddr.sin_addr), msg);
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
	
	//FIONBIO => FInputOutput NonBlocking InputOutput
	//listenSock이 사용하는 모든 API함수를 다 NonBlocking함수로 바꿔줌.
	//clientSock이 NonBlocking되는 건 아니다.
	//예외처리를 해줘야한다. 정상적인 반환인지 그냥 반환인지 구분해야한다.
	u_long on = TRUE;
	ioctlsocket(listenSock, FIONBIO, &on);

	map<int, TUser> g_userList;
	int g_iNumUser = 0;
	while (1)
	{
		// client socket 준비
		// while(1)
		//  client socket = accept(listen socket) //누군가 ->접속 시도, 연결 시도 -> 수용, 허가
		TUser tUser;
		int iSize = sizeof(tUser.clientAddr);
		tUser.sock = accept(listenSock, (sockaddr*)&tUser.clientAddr, &iSize);

		//할일이 없어서 반환되도 SOCKET_ERROR가 뜨고, 진짜 Error이여도 SOCKET_ERROR 이다.
		if (tUser.sock == SOCKET_ERROR)
		{
			//WSA_E_WOULD_BLOCK = 할일이 없어서 반환된 ERROR
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				exit(1);
			}
		}
		else
		{
			g_userList.insert(make_pair(g_iNumUser++, tUser));
			//g_userList.insert(make_pair(tUser.sock, tUser));

			//Use inet_ntop() or InetNtop()	header file <ws2ctpip.h>
			char ip[INET_ADDRSTRLEN] = { 0, };
			printf("\n ip = %s, port = %d",
				inet_ntop(AF_INET, &tUser.clientAddr.sin_addr, ip, INET_ADDRSTRLEN),
				ntohs(tUser.clientAddr.sin_port));
		}

		char recvMsg[256] = { 0, };
		int iRecvByte = sizeof(recvMsg);

		map<int, TUser>::iterator iter;
		int iByte;
		for (iter = g_userList.begin(); iter != g_userList.end(); iter++)
		{
			TUser* pUser = &(iter->second);

			/*printf("\n ip = %s, 보내라~~~", inet_ntoa(pUser->clientAddr.sin_addr));*/
			iByte = recv(pUser->sock, recvMsg, iRecvByte, 0);

			//누군가가 나갔을 경우.
			//map 에서 지우고 소켓을 닫는다.
			if (iByte == 0)
			{
				g_userList.erase(iter);
				closesocket(pUser->sock);
				tMSG(pUser, "님이 나갔습니다\n");
				break;
			}
			if (iByte == SOCKET_ERROR)
			{
				//WSA_E_WOULD_BLOCK = 할일이 없어서 반환된 ERROR
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					g_userList.erase(iter);
					closesocket(pUser->sock);
					break;
				}
			}
			else
			{
				tMSG(pUser, recvMsg);
				/*printf("\n ip = %s, msg = %s",
					inet_ntoa(pUser->clientAddr.sin_addr), recvMsg);*/
				break;
				//send(pUser->sock, recvMsg, iByte, 0);
			}
		}
		for (iter = g_userList.begin(); iter != g_userList.end(); iter++)
		{
			TUser* pUser = &(iter->second);
			send(pUser->sock, recvMsg, iByte, 0);
		}
	}
	
	while (0 < g_userList.size())
	{
		char msg[] = "게임종료합니다.!!!";
		int iRecvByte = sizeof(msg);

		for (int iUser = 0; iUser < g_userList.size(); iUser++)
		{
			send(g_userList[iUser].sock, msg, iRecvByte, 0);
		}

		for (int iUser = 0; iUser < g_userList.size(); iUser++)
		{
			closesocket(g_userList[iUser].sock);
		}
	}

	closesocket(listenSock);
	EndWinSock();
	return 0;
}

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