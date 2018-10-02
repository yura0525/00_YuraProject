#include <winsock2.h>
#include <iostream>
#include <vector>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

//#define _WINSOCK_DEPRECATED_NO_WARNINGS
vector<SOCKET> g_userList;
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


	//���ε�
	int ret = bind(sock, (sockaddr*)&addr,sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		return 1;
	}

	ret = listen(sock, SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		return 1;
	}

	SOCKET clientSock;
	SOCKADDR_IN clientInfo;
	int addlen = sizeof(clientInfo);

	while (1)
	{
		//3��° �������ڰ� sizeof(clientInfo)�� �ƴ϶� �׺����� �ּҰ�(&addlen)�� ���Ѵ�.
		clientSock = accept(sock, (sockaddr*)&clientInfo, &addlen);

		if (clientSock == INVALID_SOCKET)
		{
			break;
		}
		printf("\n %s, %d", inet_ntoa(clientInfo.sin_addr), ntohs(clientInfo.sin_port));
		g_userList.push_back(clientSock);

		//char buffer[] = "�ȳ�!";
		//send(clientSock, buffer, sizeof(buffer), 0);

		char buffer2[256] = { 0, };
		while (1)
		{
			ZeroMemory(buffer2, sizeof(char) * 256);
			ret = recv(clientSock, buffer2, sizeof(buffer2), 0);
			if (ret == 0)
			{
				printf("\n �������� %s, %d �������ϴ�.",
					inet_ntoa(clientInfo.sin_addr), ntohs(clientInfo.sin_port));
				break;
			}
			if ( ret < 0 )
			{
				printf("\n ���������� %s, %d �������ϴ�.",
					inet_ntoa(clientInfo.sin_addr), ntohs(clientInfo.sin_port));
				break;
			}
			printf("\n[%s] : ip : %s, port : %d �޾ҽ��ϴ�.",
				buffer2,
				inet_ntoa(clientInfo.sin_addr), ntohs(clientInfo.sin_port));

			send(clientSock, buffer2, sizeof(buffer2), 0);

			printf("\n[%s] : %zd ����Ʈ�� �����Ͽ����ϴ�.", buffer2, strlen(buffer2));
			/*for (int iUser = 0; iUser < g_userList.size(); iUser++)
			{
				send(g_userList[iUser], buffer2, sizeof(buffer2), 0);
			}*/
		}
	}
	//��ü �Ҹ�.
	WSACleanup();
	std::cout << "Server Hello World\n";
}