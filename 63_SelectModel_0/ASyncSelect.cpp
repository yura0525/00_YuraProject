/*
#include "Sample.h"
#include <algorithm>

#define WM_KGCA (WM_USER+0)
#define WM_RWC (WM_USER+1)

int RecvData(TUser* pUser)
{
	char buffer[256] = { 0, };
	int recvByte = 0;
	SOCKET sock = pUser->sock;
	int bConnect = true;

	int iRet = recv(sock, &buffer[recvByte], sizeof(char) * PACKET_HEADER_SIZE - recvByte, 0);
	
	//�߿�!!!!
	//������ŷ�����ϰ�� ��ó���� ���༭ ���� �޼����� �ٽ� ó���ؾ��Ѵ�.
	//if (iRet != WSAEWOULDBLOCK)
	//{
	//	PostMessage(hWnd, WM_KGCA, sock, FD_READ);
	//}

	if (CheckReturn(iRet) <= 0)
	{
		printf("\n---->��������\n");
		return -1;
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
			int iRecvByte = recv(sock, &(packet.msg[rByte]), sizeof(char)*(packet.ph.len - rByte), 0);
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
				printf("\n��Ŷ �ϼ�-> %s\n", packet.msg);
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
			printf("\n��Ŷ �ϼ�-> %s\n", packet.msg);
		}
	}

	if (bConnect == false)
		return -1;
	return 1;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KGCA)
	{
		if (WSAGETSELECTERROR(lParam))
		{
			closesocket(wParam);
		}
		else
		{
			switch (WSAGETSELECTEVENT(lParam))
			{
			case FD_ACCEPT:
			{
				TUser addUser;
				addUser.sock = accept(wParam,
					(SOCKADDR*)&addUser.clientAddr, &addUser.addrlen);
				if (addUser.sock == INVALID_SOCKET)
				{
					break;
				}
				AddUser(addUser);
				//WSAAsyncSelect���� �Լ��� ������ ȣ���ϸ� �������� ����ȴ�.
				//WM_DROP�� �ȴ�.
				int iRet = WSAAsyncSelect(addUser.sock, hWnd, WM_RWC, FD_READ | FD_WRITE | FD_CLOSE);

				if (iRet == SOCKET_ERROR)
				{
					return -1;
				}
			}
			break;
			case FD_READ:
			{
				TUser::g_socket = wParam;

				ITOR itor = find_if(g_allUser.begin(), g_allUser.end(), TUser());
				TUser* pUser = (TUser*)&(*itor);
				//Ŭ���̾�Ʈ ������ ��� recv()
				if (RecvData((TUser*)&(*itor)) <= 0)
				{
					closesocket(pUser->sock);
					itor = g_allUser.erase(itor);
				}
			}
			break;
			case FD_WRITE:
			{
				//���� ��Ŷ�� �־�δ� ��Ұ� ����.
				//��ŶǮ
			}
			break;
			case FD_CLOSE:
			{
				//���������Ͱ� 0�ϰ�쿡 ������ ó���� �Ǿ��ִ�.
				PostMessage(hWnd, WM_KGCA, wParam, FD_READ);
			}
			break;
			}
		}
	}
	else if (uMsg == WM_RWC)
	{
		int kkk = 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
HWND MakeWindow()
{
	//������ Ŭ���� ���
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = NULL;
	wc.lpszClassName = L"MyWindow";
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	//wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.hIconSm = LoadIcon(NULL, IDI_HAND);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	if (RegisterClassEx(&wc) == FALSE)
	{
		return NULL;
	}

	//������ ����(��ϵ� Ŭ������ ����ؼ�)
	HWND m_hWnd = CreateWindowEx(WS_EX_APPWINDOW,
		L"MyWindow", NULL, NULL,
		0, 0, 800, 600, NULL, NULL, NULL, NULL);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return m_hWnd;
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
	HWND hWnd = MakeWindow();

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

	int iRet = WSAAsyncSelect(listenSock, hWnd, WM_KGCA, FD_ACCEPT | FD_CLOSE);
	if (iRet == SOCKET_ERROR)
	{
		return -1;
	}

	MSG msg;
	while (iRet = GetMessage(&msg, NULL, 0, 0))
	{
		if (iRet == -1)
			break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	closesocket(listenSock);
	EndWinSock();

	//DeleteCriticalSection(&g_Crit);
	CloseHandle(g_hMutex);
	return 0;
}
*/