#pragma once
#include <windows.h>

//패킷은 1바이트로 패킹
#pragma pack(push, 1)

typedef struct
{
	WORD len;
	WORD type;
}PACKET_HEADER;

typedef struct
{
	PACKET_HEADER ph;
	char msg[2048];
}UPACKET, *P_UPACKET;

typedef struct
{
	byte szName[13];
	int iIndex;
	int iData[20];
}CHARACTER_INFO;

//다시 원래의 값으로 되돌려준다.
#pragma pack(pop)


#define PACKET_HEADER_SIZE 4

// TYPE
#define PACKET_CHAT_MSG 1000
#define PACKET_CREATE_CHARACTER 2000

// SEND
//char buffer[] = "kgca";
//
//UPACKET packet;
//ZeroMemory(&packet, sizeof(UPACK));
//packet.ph.len = PACKET_HEADER_SIZE + strlen(buffer);
//packet.ph.type = PACKET_CHAT_MSG;
//
//memcpy(packet.msg, buffer, strlen(buffer));
//
////패킷은 1바이트로 패킹되어서 char형태로 컨버팅 가능하다.
//send(sock, (char*)&packet, packet.ph.len, 0);


//UPACKET packet;
//ZeroMemory(&packet, sizeof(UPACK));
//PACKET_CREATE_CHARACTER cInfo;
//cInfo.szName = ;
//cInfo.iIndex = ;
//cInfo.iData = ;
//packet.ph.len = PACKET_HEADER_SIZE + strlen(cInfo);
//packet.ph.type = PACKET_CREATE_CHARACTER;
//
//memcpy(packet.msg, (char*)cInfo, strlen(cInfo));
//
////패킷은 1바이트로 패킹되어서 char형태로 컨버팅 가능하다.
//send(sock, (char*)&packet, packet.ph.len, 0);

//RECV
//char buffer[256] = {0,};
//int recvByte = 0;
//while (1)
//{
//	/*UPACKET packet;
//	ZeroMemory(&packet, sizeof(UPACK));
//	recvByte += recv(sock, (char*)packet.ph[recvByte], sizeof(char) * PACKET_HEADER - recvByte, 0);*/
//
//	recvByte += recv(sock, buffer[recvByte], sizeof(char) * PACKET_HEADER - recvByte, 0);
//	if (recvByte == PACKET_HEADER)
//	{
//		UPACKET packet;
//		ZeroMemory(&packet, sizeof(UPACK));
//		memcpy(packet.ph, buffer, sizeof(char) * PACKET_HEADER);
//		int rByte = 0;
//		do
//		{
//			rByte += recv(sock, pack.msg[rByte], sizeof(char)*(packet.ph.len - rByte), 0);
//		} while (packet.ph.len > rByte);
//
//		recvByte = 0;
//		rByte = 0;
//
//		switch (packet.ph.type)
//		{
//		case PACKET_CHAT_MSG:
//		{
//			printf("패킷 완성 %s", packet.msg);
//		}
//		break;
//		case PACKET_CREATE_CHARACTER:
//		{
//			CHARACTER_INFO cInfo;
//			memcpy(cInfo, packet.msg, sizeof(CHARACTER_INFO));
//		}
//		break;
//		}
//		printf("패킷 완성 %s", packet.msg);
//	}
//}
