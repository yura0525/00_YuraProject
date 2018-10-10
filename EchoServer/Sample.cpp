#include <winsock2.h>
#include <ws2tcpip.h>	//InetNtop(), inet_ntop();
#include <iostream>
#include <map>
#include "TProtocol.h"

#pragma comment(lib, "ws2_32")
using namespace std;

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
	do {
		send(sock, (char*)&pMsg[sendbytes], iTotalsize - sendbytes, 0);
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
	do {
		send(sock, (char*)&pMsg[sendbytes], iTotalsize - sendbytes, 0);
	} while (sendbytes < iTotalsize);

	return iTotalsize;
}

int SendMsg(SOCKET sock, UPACKET* uPacket)
{
	int sendbytes = 0;
	int iTotalsize = strlen(uPacket->msg) + PACKET_HEADER_SIZE;
	char* pMsg = (char*)uPacket->msg;
	do {
		send(sock, (char*)&pMsg[sendbytes], iTotalsize - sendbytes, 0);
	} while (sendbytes < iTotalsize);

	return iTotalsize;
}
DWORD WINAPI ClientThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;

	char buffer[256] = { 0, };
	int recvByte = 0;
	while (1)
	{
		/*UPACKET packet;
		ZeroMemory(&packet, sizeof(UPACK));
		recvByte += recv(sock, (char*)packet.ph[recvByte], sizeof(char) * PACKET_HEADER - recvByte, 0);*/

		recvByte += recv(sock, &buffer[recvByte], sizeof(char) * PACKET_HEADER_SIZE - recvByte, 0);
		if (recvByte == 0) break;

		if (recvByte == PACKET_HEADER_SIZE)
		{
			UPACKET packet;
			ZeroMemory(&packet, sizeof(UPACKET));
			memcpy(&packet.ph, buffer, sizeof(char) * PACKET_HEADER_SIZE);

			int rByte = 0;
			do
			{
				rByte += recv(sock,&(packet.msg[rByte]), sizeof(char)*(packet.ph.len - rByte), 0);
				if (rByte == 0) break;
			} while (packet.ph.len > rByte);

			rByte = 0;

			switch (packet.ph.type)
			{
			case PACKET_CHAT_MSG:
			{
				printf("��Ŷ �ϼ� %s", packet.msg);
				SendMsg(sock, packet.msg, PACKET_CHAT_MSG);
				//SendMsg(sock, &packet);
				//SendMsg(sock, packet.ph, packet.msg);
			}
			break;
			case PACKET_CREATE_CHARACTER:
			{
				CHARACTER_INFO cInfo;
				memcpy(&cInfo, packet.msg, sizeof(CHARACTER_INFO));
			}
			break;
			}
			printf("��Ŷ �ϼ� %s", packet.msg);

			recvByte = 0;
		}
	}
	closesocket(sock);
	return 1;
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

	while (1)
	{
		// client socket �غ�
		// while(1)
		//  client socket = accept(listen socket) //������ ->���� �õ�, ���� �õ� -> ����, �㰡

		SOCKADDR_IN clientaddr;
		int addlen = sizeof(clientaddr);
		SOCKET client = accept(listenSock, (sockaddr*)&clientaddr, &addlen);

		if( client != SOCKET_ERROR )
		{
			//Use inet_ntop() or InetNtop()	header file <ws2ctpip.h>
			char ip[INET_ADDRSTRLEN] = { 0, };
			printf("\n ip = %s, port = %d",
				inet_ntop(AF_INET, &clientaddr.sin_addr, ip, INET_ADDRSTRLEN),
				ntohs(clientaddr.sin_port));
		}

		DWORD threadID;
		HANDLE hThread = CreateThread( 0, 0, ClientThread, (LPVOID)client, 0, &threadID);
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