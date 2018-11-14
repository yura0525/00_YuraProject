#include "Sample.h"

HANDLE g_hEvent;
DWORD WINAPI SendThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;

	char buffer[256] = { 0, };
	while (1)
	{
		printf("\n���� ������ �Է��Ͻÿ�?\n");
		fgets(buffer, 256, stdin);
		if (strlen(buffer) <= 1)
		{
			break;
		}
		// enter����.
		if (buffer[strlen(buffer) - 1] == '\n')
			buffer[strlen(buffer) - 1] = 0;

		int iSendByte = SendMsg(sock, buffer, PACKET_CHAT_MSG);
		if (iSendByte == SOCKET_ERROR)	break;
		printf("\n[%s] : %zd ����Ʈ�� �����Ͽ����ϴ�.\n", buffer, strlen(buffer));
	}
	shutdown(sock, SD_BOTH);
	closesocket(sock);
	return 1;
}

DWORD WINAPI RecvThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;

	char buffer[2048] = { 0, };
	
	int iRecvByte = 0;
	int iStartByte = 0;
	ZeroMemory(buffer, sizeof(char) * 2048);

	bool bConnect = true;

	while (bConnect)
	{
		ZeroMemory(buffer, sizeof(char) * 2048);
		int iLen = 0;
		iLen = recv(sock, &buffer[iStartByte], sizeof(char) * PACKET_HEADER_SIZE - iRecvByte, 0);
		iRecvByte += iLen;
		if (iLen == 0)
		{
			printf("\n---->��������\n");
			break;
		}
		if (iLen <= SOCKET_ERROR)
		{
			printf("\n---->������ ����\n");
			break;
		}

		if (iRecvByte == PACKET_HEADER_SIZE)
		{
			UPACKET* pPacket = (UPACKET*)&buffer;
			int iRecvMsg = 0;
			while (iRecvMsg < pPacket->ph.len);
			{
				iRecvMsg = recv(sock, (char*)&buffer[iRecvByte],
					sizeof(char) * pPacket->ph.len, 0);
				iRecvByte += iRecvMsg;
				if (iRecvMsg == 0 || iRecvMsg == SOCKET_ERROR)
				{
					printf("\niLen == 0 || iLen == SOCKET_ERROR [��������] : %s", "���� ������ ���Ͽ� ����Ǿ���.\n");
					bConnect = false;
					break;
				}
			}

			UPACKET recvmsg;
			memset(&recvmsg, 0, sizeof(recvmsg));
			memcpy(&recvmsg, pPacket, pPacket->ph.len);
			switch (recvmsg.ph.type)
			{
				//case PACKET_JOIN;
				//case PACKET_DROP;
				case PACKET_CHAT_MSG:
				{
					printf("\n[�����޼���]: %s\n", recvmsg.msg);
				}
				break;
			}
			iStartByte = iRecvByte = 0;
		}
		else
		{
			iStartByte += iRecvByte;
		}
	}
	shutdown(sock, SD_BOTH);
	closesocket(sock);
	return 1;
}

DWORD WINAPI ConnectThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("192.168.0.27");
	//addr.sin_addr.s_addr = inet_addr("192.168.0.101");
	addr.sin_port = htons(10000);

	int ret = connect(sock, (sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		return 1;
	}

	//�̺�Ʈ�� ���¸� �ñ׳� ���°� �ǰ��Ѵ�.
	SetEvent(g_hEvent);		
	return 1;
}
int main()
{
	WSADATA wd;
	//���� �ʱ�ȭ. 0�̸� ����
	if (BeginWinSock() == false)
	{
		return 1;
	}

	// �̺�Ʈ�� �����尣 �߰�, �����尣 �帧�� �����Ϸ��� ����Ѵ�.
	// �̺�Ʈ ��ü�� ������ �Ͼ���� �˸��� ����ȭ ��ü�̴�.
	// �����尣�� �۾� ������ �ñ⸦ �����ϰ� ��ȣ�� ������ ���� ���Ǹ�,
	// �������� �޼����� ������ ����� �����ִ�.
	// �ڵ� ���� �̺�Ʈ : ��� ���°� ����Ǹ� �ڵ����� ���ȣ ���°� �ȴ�.
	// ���� ���� �̺�Ʈ : �����尡 ���ȣ ���·� ���鶧���� ��ȣ ���¸� �����Ѵ�.
	// 2 : FALSE(�ڵ����� �̺�Ʈ ), TRUE( �������� �̺�Ʈ )
	// 3 : �ʱⰪ(FALSE: ��ñ׳�, TRUE:�ñ׳�)
	//�ڵ����� �̺�Ʈ�� ��� �� �ص� �ǰ�, �������� �̺�Ʈ�� �ʿ��ϴ�.
	//�̺�Ʈ�� ���¸� ��ñ׳� ���°� �ǰ��Ѵ�. => ResetEvent(g_hEvent);
	//�̺�Ʈ�� ���¸� �ñ׳� ���°� �ǰ��Ѵ�.	=> SetEvent(g_hEvent);
	
	//�ʱⰪ�� ��ñ׳ε� ����.
	g_hEvent = CreateEvent(NULL, FALSE,
		FALSE,
		L"ConnectEvent");

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET)
	{
		return 1;
	}

	//���� ���� �Ϸ�.
	/*SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);*/
	DWORD id_0;
	HANDLE hConnectThread = CreateThread(0, 0, ConnectThread, (LPVOID)sock, 0, &id_0);

	//event
	// Ŀ��Ʈ�� �Ǿ� ���� ���ú� �Ǵϱ� Ŀ��Ʈ�� �ɶ����� ����Ϸ��� �̺�Ʈ�� ����Ѵ�
	//Ŀ��Ʈ�� �Ǹ� �̺�Ʈ�� �ñ׳� ���°� �ȴ�.
	WaitForSingleObject(g_hEvent, INFINITE);

	DWORD id_1;
	HANDLE hSendThread = CreateThread(NULL, 0, SendThread, (LPVOID)sock, 0, &id_1);

	DWORD id_2;
	HANDLE hRecvThread = CreateThread(NULL, 0, RecvThread, (LPVOID)sock, 0, &id_2);

	// �����尡 ����Ǿ����� Ȯ��
	WaitForSingleObject(hSendThread, INFINITE);
	WaitForSingleObject(hRecvThread, INFINITE);

	closesocket(sock);
	CloseHandle(hConnectThread);
	CloseHandle(hSendThread);
	CloseHandle(hRecvThread);
	CloseHandle(g_hEvent);

	EndWinSock();

	std::cout << "\nClient Hello World\n";
	getchar();
}