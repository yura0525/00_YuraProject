#include "Sample.h"

DWORD WINAPI ClientThread(LPVOID arg)
{
	SOCKET* sock = (SOCKET*)arg;
	TUser User = g_allUser[(*sock)];

	char buffer[256] = { 0, };
	int recvByte = 0;
	int iRet = 0;
	bool bConnect = true;

	while (bConnect)
	{
		iRet = recv(*sock, &buffer[recvByte], sizeof(char) * PACKET_HEADER_SIZE - recvByte, 0);
		if (CheckReturn(iRet) <= 0)
		{
			printf("\n---->정상퇴장\n");
			break;
		}
		recvByte += iRet;

		if (recvByte == PACKET_HEADER_SIZE)
		{
			UPACKET packet;
			ZeroMemory(&packet, sizeof(UPACKET));
			memcpy(&packet.ph, buffer, sizeof(char) * PACKET_HEADER_SIZE);

			int rByte = 0;
			do
			{
				int iRecvByte = recv(*sock, &(packet.msg[rByte]), sizeof(char)*(packet.ph.len - rByte), 0);
				if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
				{
					bConnect = false;
					T_ERROR();
					break;
				}
				rByte += iRecvByte;
			} while (packet.ph.len > rByte);

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
				}
				break;
				}
				printf("패킷 완성 %s\n", packet.msg);
			}
		}
		Sleep(1);
	}
	DelUser(&User);
	closesocket(*sock);
	return 1;
}

int main()
{
	//InitializeCriticalSection(&g_Crit);
	g_hMutex = CreateMutex(NULL, FALSE, L"KGCA");

	//ERROR_ALREADY_EXISTS : 프로세스가 2개 중복 생성되는걸 막는다.
	//프로세스가 실행된지 모르고 2개 실행될때, 이미 실행되어있다고 막는다.
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(g_hMutex);
		return 1;
	}
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

		TUser user(client, clientaddr);
		AddUser(user);

		DWORD threadID;
		HANDLE hThread = CreateThread(0, 0, ClientThread, (LPVOID)&client, 0, &threadID);

		if (client != SOCKET_ERROR)
		{
			//Use inet_ntop() or InetNtop()	header file <ws2ctpip.h>
			char ip[INET_ADDRSTRLEN] = { 0, };
			printf("\n입장!!!!! ip = %s, port = %d\n",
				inet_ntop(AF_INET, &clientaddr.sin_addr, ip, INET_ADDRSTRLEN),
				ntohs(clientaddr.sin_port));
		}
	}

	closesocket(listenSock);
	EndWinSock();

	//DeleteCriticalSection(&g_Crit);
	CloseHandle(g_hMutex);
	return 0;
}