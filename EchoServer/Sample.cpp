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
				printf("패킷 완성 %s", packet.msg);
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
			printf("패킷 완성 %s", packet.msg);

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

	while (1)
	{
		// client socket 준비
		// while(1)
		//  client socket = accept(listen socket) //누군가 ->접속 시도, 연결 시도 -> 수용, 허가

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