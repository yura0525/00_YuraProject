#include <winsock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

//#define _WINSOCK_DEPRECATED_NO_WARNINGS

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

	int ret = connect(sock, (sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		return 1;
	}

	char buffer[256] = { 0, };
	recv(sock, buffer, sizeof(buffer), 0);
	std::cout << buffer<<std::endl;

	char buffer2[256] = { 0, };
	//send(sock, buffer2, sizeof(buffer2), 0);
	int iLen = 0;
	while (1)
	{
		ZeroMemory(buffer2, sizeof(char) * 256);
		fgets(buffer2, 256, stdin);
		
		//enter지움.
		buffer2[strlen(buffer2)-1] = '\0';

		send(sock, buffer2, strlen(buffer2), 0);
		printf("[%s] : %d 바이트를 전송하였습니다.\n", buffer2, strlen(buffer2));

		ZeroMemory(buffer2, sizeof(char) * 256);
		recv(sock, buffer2, sizeof(buffer2), 0);
		printf("[%s] : %d 바이트를 받았습니다.\n", buffer2, strlen(buffer2));
	}
	
	closesocket(sock);
	WSACleanup();
	std::cout << "Client Hello World\n";
}