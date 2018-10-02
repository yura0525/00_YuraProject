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
	addr.sin_addr.s_addr = inet_addr("192.168.0.27");
	addr.sin_port = htons(10000);


	//바인딩
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
		//3번째 전달인자가 sizeof(clientInfo)가 아니라 그변수의 주소값(&addlen)을 원한다.
		clientSock = accept(sock, (sockaddr*)&clientInfo, &addlen);

		if (clientSock == INVALID_SOCKET)
		{
			break;
		}
		printf("\n %s, %d", inet_ntoa(clientInfo.sin_addr), ntohs(clientInfo.sin_port));
		g_userList.push_back(clientSock);

		//char buffer[] = "안녕!";
		//send(clientSock, buffer, sizeof(buffer), 0);

		char buffer2[256] = { 0, };
		while (1)
		{
			ZeroMemory(buffer2, sizeof(char) * 256);
			ret = recv(clientSock, buffer2, sizeof(buffer2), 0);
			if (ret == 0)
			{
				printf("\n 정상종료 %s, %d 나갔습니다.",
					inet_ntoa(clientInfo.sin_addr), ntohs(clientInfo.sin_port));
				break;
			}
			if ( ret < 0 )
			{
				printf("\n 비정상종료 %s, %d 나갔습니다.",
					inet_ntoa(clientInfo.sin_addr), ntohs(clientInfo.sin_port));
				break;
			}
			printf("\n[%s] : ip : %s, port : %d 받았습니다.",
				buffer2,
				inet_ntoa(clientInfo.sin_addr), ntohs(clientInfo.sin_port));

			send(clientSock, buffer2, sizeof(buffer2), 0);

			printf("\n[%s] : %zd 바이트를 전송하였습니다.", buffer2, strlen(buffer2));
			/*for (int iUser = 0; iUser < g_userList.size(); iUser++)
			{
				send(g_userList[iUser], buffer2, sizeof(buffer2), 0);
			}*/
		}
	}
	//객체 소멸.
	WSACleanup();
	std::cout << "Server Hello World\n";
}