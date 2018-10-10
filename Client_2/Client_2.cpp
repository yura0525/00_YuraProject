#include <winsock2.h>
#include <iostream>
#include"TProtocol.h"

#pragma comment(lib, "ws2_32.lib")

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
		sendbytes += send(sock, (char*)&pMsg[sendbytes], iTotalsize - sendbytes, 0);
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
		sendbytes += send(sock, (char*)&pMsg[sendbytes], iTotalsize - sendbytes, 0);
	} while (sendbytes < iTotalsize);

	return iTotalsize;
}

int SendMsg(SOCKET sock, UPACKET* uPacket)
{
	int sendbytes = 0;
	int iTotalsize = strlen(uPacket->msg) + PACKET_HEADER_SIZE;
	char* pMsg = (char*)uPacket->msg;
	do {
		sendbytes += send(sock, (char*)&pMsg[sendbytes], iTotalsize - sendbytes, 0);
	} while (sendbytes < iTotalsize);

	return iTotalsize;
}


//#define _WINSOCK_DEPRECATED_NO_WARNINGS
DWORD WINAPI SendThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	char buffer[256] = { 0, };
	while (1)
	{
		fgets(buffer, 256, stdin);
		if (strlen(buffer) <= 1)
		{
			break;
		}
		// enter지움.
		if (buffer[strlen(buffer) - 1] == '\n')
			buffer[strlen(buffer) - 1] = 0;

		int iSendByte = SendMsg(sock, buffer, PACKET_CHAT_MSG);
		if (iSendByte == SOCKET_ERROR)	break;
		printf("[%s] : %zd 바이트를 전송하였습니다.\n", buffer, strlen(buffer));
	}

	closesocket(sock);
	return 1;
}

DWORD WINAPI RecvThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char buffer[2048] = { 0, };
	printf("\n보낼 데이터 입력하시오?");
	int iRecvByte = 0;
	int iStartByte = 0;
	ZeroMemory(buffer, sizeof(char) * 2048);

	bool bConnect = true;

	while (bConnect)
	{
		int iLen = 0;
		iLen = recv(sock, &buffer[iStartByte], PACKET_HEADER_SIZE - iRecvByte, 0);
		iRecvByte += iLen;

		if (iLen == 0 || iLen == SOCKET_ERROR)
		{
			printf("[연결종료] : %s", "서버 문제로 인하여 종료되었다.");
			break;
		}
		if (iRecvByte == PACKET_HEADER_SIZE)
		{
			UPACKET* pPacket = (UPACKET*)&buffer;

			while (iRecvByte < pPacket->ph.len)
			{
				iLen = recv(sock, &buffer[iRecvByte], pPacket->ph.len - iRecvByte, 0);
				iRecvByte += iLen;

				if (iLen == 0 || iLen == SOCKET_ERROR)
				{
					printf("[연결종료] : %s", "서버 문제로 인하여 종료되었다.");
					break;
				}
			}

			if (iRecvByte == pPacket->ph.len)
			{
				UPACKET recvmsg;
				memset(&recvmsg, 0, sizeof(recvmsg));
				memcpy(&recvmsg, pPacket, pPacket->ph.len);
				switch (recvmsg.ph.type)
				{
				case PACKET_CHAT_MSG:
				{
					printf("\n[받은메세지]: %s", recvmsg.msg);
					printf("\n보낼 데이터 입력하시오?");
				}break;
				}
				iStartByte = iRecvByte = 0;
			}
		}
		else
		{
			iStartByte += iRecvByte;
		}
	}
	closesocket(sock);
	return 1;
}
int main()
{
	WSADATA wd;
	//윈속 초기화. 0이면 성공
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		return 1;
	}

	//소켓 생성 완료.
	/*SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);*/
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET)
	{
		return 1;
	}

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	//addr.sin_addr.s_addr = inet_addr("192.168.0.27");
	addr.sin_addr.s_addr = inet_addr("192.168.0.101");
	addr.sin_port = htons(10000);

	int ret = connect(sock, (sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		return 1;
	}


	//recv(sock, buffer, sizeof(buffer), 0);
	//std::cout << buffer<<std::endl;

	//char buffer2[256] = { 0, };
	int iLen = 0;

	DWORD id;
	HANDLE hThread = CreateThread(NULL, 0, SendThread, (LPVOID)sock, 0, &id);
	int recvByte = 0;
	char buffer[256] = { 0, };
	while (1)
	{
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
				rByte += recv(sock, &(packet.msg[rByte]), sizeof(char)*(packet.ph.len - rByte), 0);
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
			}
			//printf("패킷 완성 %s", packet.msg);
		}
	}

	closesocket(sock);
	WSACleanup();
	std::cout << "Client Hello World\n";
	getchar();
}