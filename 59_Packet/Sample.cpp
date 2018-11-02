#include <winsock2.h>
#include "TPacket.h"
#pragma comment(lib, "ws2_32")

//#include <winsock2.h> 헤더파일은 #include <windows.h>보다 위에 인클루드해야한다!!!!!
//안그러면 많은 에러가 난다.
int SendMsg(SOCKET sock, const char* msg, WORD type)
{
	string address = "서울특별시";
	TPacket sendPacket(type);
	//packet << msg << 1000;
	sendPacket << msg;
	sendPacket << 1000;
	sendPacket << address;
	int iRet = send(sock, (char*)&sendPacket.m_uPacket, sendPacket.m_uPacket.ph.len + PACKET_HEADER_SIZE, 0);
	
	char name[5] = { 0, };
	int iValue = 0;
	string add;
	TPacket recvPacket;
	recvPacket.m_uPacket.ph = sendPacket.m_uPacket.ph;
	memcpy(recvPacket.m_uPacket.msg, sendPacket.m_uPacket.msg, sizeof(char) * 256);
	//packet << msg << 1000;
	recvPacket >> name;
	recvPacket >> iValue;
	recvPacket >> add;

	return iRet;
}

void main()
{
	//TPacket data;
	//data << "홍길동";
	//data << age;

	SOCKET sock = 0;
	SendMsg(sock, "game", PACKET_CHAT_MSG);
	//|[HEADER] + "홍길동" + age
}