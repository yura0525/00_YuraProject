#include <winsock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

//#define _WINSOCK_DEPRECATED_NO_WARNINGS
DWORD WINAPI SendThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	while (1)
	{
		char buffer[256] = { 0, };
		fgets(buffer, 256, stdin);
		if (strlen(buffer) <= 1)
		{
			break;
		}
		// enter����.
		buffer[strlen(buffer) - 1] = '\0';

		send(sock, buffer, strlen(buffer), 0);
		printf("[%s] : %zd ����Ʈ�� �����Ͽ����ϴ�.\n", buffer, strlen(buffer));
	}

	closesocket(sock);
	return 1;
}
int main()
{
	WSADATA wd;
	//���� �ʱ�ȭ. 0�̸� ����
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		return 1;
	}

	//���� ���� �Ϸ�.
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
	addr.sin_addr.s_addr = inet_addr("192.168.0.27");
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

	while (1)
	{
		char buffer[256] = { 0, };
		int iRet = recv(sock, buffer, sizeof(buffer), 0);

		if (iRet == 0)
		{
			break;
		}
		if (iRet == SOCKET_ERROR)
		{
			closesocket(sock);
			break;
		}
		printf("[%s] : %zd ����Ʈ�� �޾ҽ��ϴ�.\n", buffer, strlen(buffer));
	}

	closesocket(sock);
	WSACleanup();
	std::cout << "Client Hello World\n";
	getchar();
}