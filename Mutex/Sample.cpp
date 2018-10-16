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
			printf("\n---->��������\n");
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
					printf("��Ŷ �ϼ� %s\n", packet.msg);
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
				printf("��Ŷ �ϼ� %s\n", packet.msg);
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

	//ERROR_ALREADY_EXISTS : ���μ����� 2�� �ߺ� �����Ǵ°� ���´�.
	//���μ����� ������� �𸣰� 2�� ����ɶ�, �̹� ����Ǿ��ִٰ� ���´�.
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

	while (1)
	{
		// client socket �غ�
		// while(1)
		//  client socket = accept(listen socket) //������ ->���� �õ�, ���� �õ� -> ����, �㰡

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
			printf("\n����!!!!! ip = %s, port = %d\n",
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