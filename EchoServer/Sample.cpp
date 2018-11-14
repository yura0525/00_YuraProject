#include <winsock2.h>
#include <ws2tcpip.h>	//InetNtop(), inet_ntop();
#include <iostream>
#include <map>
#include "TProtocol.h"

#pragma comment(lib, "ws2_32")
using namespace std;
struct TUser
{
	SOCKET sock;
	SOCKADDR_IN clientAddr;
	int iIndex;
};
TUser*	g_allUser[100];
int		g_iNumClient = 0;
CRITICAL_SECTION g_Crit;

void AddUser(TUser* user)
{
	EnterCriticalSection(&g_Crit);
		g_allUser[g_iNumClient] = user;
		g_iNumClient++;
	LeaveCriticalSection(&g_Crit);
}

bool BeginWinSock();
bool EndWinSock();

int SendMsg(SOCKET sock, char* msg, WORD type)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sendmsg.ph.len = strlen(msg);
	sendmsg.ph.type = type;
	memcpy(sendmsg.msg, msg, strlen(msg));
	
	int sendbytes = 0;
	int iTotalsize = strlen(msg) + PACKET_HEADER_SIZE;
	char* pMsg = (char*)&sendmsg;
	int iSend = 0;
	do
	{
		iSend = send(sock, (char*)&pMsg[sendbytes], iTotalsize - sendbytes, 0);
		if (iSend == SOCKET_ERROR) { return iSend; }

		sendbytes += iSend;
	} while (sendbytes < iTotalsize);

	return iTotalsize;
}

int SendMsg(SOCKET sock, PACKET_HEADER ph, char* msg)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sendmsg.ph = ph;
	memcpy(sendmsg.msg, msg, strlen(msg));

	int sendbytes = 0;
	int iTotalsize = strlen(msg) + PACKET_HEADER_SIZE;
	char* pMsg = (char*)&sendmsg;
	int iSend = 0;
	do
	{
		iSend = send(sock, (char*)&pMsg[sendbytes], iTotalsize - sendbytes, 0);
		sendbytes += iSend;
	} while (sendbytes < iTotalsize);

	return iTotalsize;
}

int SendMsg(SOCKET sock, UPACKET* uPacket)
{
	int sendbytes = 0;
	int iTotalsize = strlen(uPacket->msg) + PACKET_HEADER_SIZE;
	char* pMsg = (char*)uPacket->msg;
	int iSend = 0;
	do
	{
		iSend = send(sock, (char*)&pMsg[sendbytes], iTotalsize - sendbytes, 0);
		sendbytes += iSend;
	} while (sendbytes < iTotalsize);

	return iTotalsize;
}
int Broadcastting(char* pMsg)
{
	EnterCriticalSection(&g_Crit);
	for (int iUser = 0; iUser < g_iNumClient; iUser++)
	{
		if (0 <= SendMsg(g_allUser[iUser]->sock, pMsg, PACKET_CHAT_MSG))
		{
			continue;
			//SendMsg(sock, &packet);
			//SendMsg(sock, packet.ph, packet.msg);
		}
	}
	LeaveCriticalSection(&g_Crit);
	return 1;
}
void DelUser(TUser* pUser)
{
	printf("\n퇴장!!!!! ip = %s, port = %d", 
		inet_ntoa(pUser->clientAddr.sin_addr), ntohs(pUser->clientAddr.sin_port));

	EnterCriticalSection(&g_Crit);
	for (int iUser = 0; iUser < g_iNumClient; iUser++)
	{
		if (g_allUser[iUser]->sock == pUser->sock)
		{
			//지울때 뒤에서 하나씩 땡겨오고 g_iNumClient을 하나 줄인다.
			for (int iDel = iUser; iDel < g_iNumClient; iDel++)
			{
				g_allUser[iDel] = g_allUser[iDel + 1];
			}
			break;
		}
	}
	g_iNumClient--;
	
	closesocket(pUser->sock);
	LeaveCriticalSection(&g_Crit);
	return;
}
DWORD WINAPI ClientThread(LPVOID arg)
{
	TUser* pUser = (TUser*)arg;
	SOCKET sock = pUser->sock;

	char buffer[256] = { 0, };
	int recvByte = 0;
	int iRet = 0;
	bool bConnect = true;

	while (1)
	{
		ZeroMemory(&buffer, sizeof(char) * 256);
		iRet = recv(sock, &buffer[recvByte], sizeof(char) * PACKET_HEADER_SIZE - recvByte, 0);

		if (iRet == 0|| iRet == SOCKET_ERROR)
			break;

		recvByte += iRet;

		if (recvByte == PACKET_HEADER_SIZE)
		{
			UPACKET packet;
			ZeroMemory(&packet, sizeof(UPACKET));
			memcpy(&packet.ph, buffer, sizeof(char) * PACKET_HEADER_SIZE);

			int rByte = 0;
			do
			{
				int iRecvByte = recv(sock,&(packet.msg[rByte]), sizeof(char)*(packet.ph.len - rByte), 0);
				if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
				{
					bConnect = false;
					break;
				}
				rByte += iRecvByte;
			} while (rByte < packet.ph.len);
			
			recvByte = 0;

			if (bConnect)
			{
				switch (packet.ph.type)
				{
					case PACKET_CHAT_MSG:
					{
						printf("패킷 완성 %s\n", packet.msg);
						//SendMsg(sock, packet.msg, PACKET_CHAT_MSG);
						//SendMsg(sock, &packet);
						//SendMsg(sock, packet.ph, packet.msg);
						Broadcastting(packet.msg);
					}
					break;
					case PACKET_CREATE_CHARACTER:
					{
						CHARACTER_INFO cInfo;
						memcpy(&cInfo, packet.msg, sizeof(CHARACTER_INFO));
						printf("패킷 완성 %s\n", packet.msg);
					}
					break;
				}
			}
		}
		Sleep(1);
	}
	DelUser(pUser);
	closesocket(sock);
	return 1;
}

int main()
{
	InitializeCriticalSection(&g_Crit);
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

	while (1)
	{
		// client socket 준비
		// while(1)
		//  client socket = accept(listen socket) //누군가 ->접속 시도, 연결 시도 -> 수용, 허가

		SOCKADDR_IN clientaddr;
		int addlen = sizeof(clientaddr);
		SOCKET client = accept(listenSock, (sockaddr*)&clientaddr, &addlen);

		TUser user;
		user.sock = client;
		user.clientAddr = clientaddr;

		AddUser(&user);

		DWORD threadID;
		HANDLE hThread = CreateThread(0, 0, ClientThread, (LPVOID)&g_allUser[g_iNumClient-1], 0, &threadID);

		if( client != SOCKET_ERROR )
		{
			//Use inet_ntop() or InetNtop()	header file <ws2ctpip.h>
			char ip[INET_ADDRSTRLEN] = { 0, };
			printf("\n입장!!!!! ip = %s, port = %d",
				inet_ntop(AF_INET, &clientaddr.sin_addr, ip, INET_ADDRSTRLEN),
				ntohs(clientaddr.sin_port));
		}
	}

	closesocket(listenSock);
	EndWinSock();

	DeleteCriticalSection(&g_Crit);
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