#include <winsock2.h>
#include "TPacket.h"
#pragma comment(lib, "ws2_32")

//#include <winsock2.h> ��������� #include <windows.h>���� ���� ��Ŭ����ؾ��Ѵ�!!!!!
//�ȱ׷��� ���� ������ ����.
int SendMsg(SOCKET sock, const char* msg, WORD type)
{
	string address = "����Ư����";
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
	//data << "ȫ�浿";
	//data << age;

	SOCKET sock = 0;
	SendMsg(sock, "game", PACKET_CHAT_MSG);
	//|[HEADER] + "ȫ�浿" + age
}