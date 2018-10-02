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
	
	//FIONBIO => FInputOutput NonBlocking InputOutput
	//listenSock�� ����ϴ� ��� API�Լ��� �� NonBlocking�Լ��� �ٲ���.
	//clientSock�� NonBlocking�Ǵ� �� �ƴϴ�.
	//����ó���� ������Ѵ�. �������� ��ȯ���� �׳� ��ȯ���� �����ؾ��Ѵ�.
	u_long on = TRUE;
	ioctlsocket(listenSock, FIONBIO, &on);

	map<int, TUser> g_userList;
	int g_iNumUser = 0;
	while (1)
	{
		// client socket �غ�
		// while(1)
		//  client socket = accept(listen socket) //������ ->���� �õ�, ���� �õ� -> ����, �㰡
		TUser tUser;
		int iSize = sizeof(tUser.clientAddr);
		tUser.sock = accept(listenSock, (sockaddr*)&tUser.clientAddr, &iSize);

		//������ ��� ��ȯ�ǵ� SOCKET_ERROR�� �߰�, ��¥ Error�̿��� SOCKET_ERROR �̴�.
		if (tUser.sock == SOCKET_ERROR)
		{
			//WSA_E_WOULD_BLOCK = ������ ��� ��ȯ�� ERROR
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

			/*printf("\n ip = %s, ������~~~", inet_ntoa(pUser->clientAddr.sin_addr));*/
			iByte = recv(pUser->sock, recvMsg, iRecvByte, 0);

			//�������� ������ ���.
			//map ���� ����� ������ �ݴ´�.
			if (iByte == 0)
			{
				g_userList.erase(iter);
				closesocket(pUser->sock);
				tMSG(pUser, "���� �������ϴ�\n");
				break;
			}
			if (iByte == SOCKET_ERROR)
			{
				//WSA_E_WOULD_BLOCK = ������ ��� ��ȯ�� ERROR
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
		char msg[] = "���������մϴ�.!!!";
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