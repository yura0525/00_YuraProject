#include <winsock2.h>
#include <ws2tcpip.h>	//InetNtop(), inet_ntop();
#include <iostream>
#include <map>
#include "TProtocol.h"

#pragma comment(lib, "ws2_32")
using namespace std;
struct TUser
{
	SOCKET sock;
	SOCKADDR_IN clientAddr;
	TUser() {}
	TUser(SOCKET client, SOCKADDR_IN add)
	{
		sock = client;
		clientAddr = add;
	}
	TUser(const TUser& user)
	{
		sock = user.sock;
		clientAddr = user.clientAddr;
	}
	int iIndex;
};

std::map<SOCKET, TUser> g_allUser;
typedef std::map<SOCKET, TUser>::iterator ITOR;

CRITICAL_SECTION g_Crit;
HANDLE			g_hMutex;

void T_ERROR()
{
	char* pMsg = 0;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&pMsg, 0, NULL);

	printf("\n%s\n", pMsg);
	LocalFree(pMsg);
}
int CheckReturn(int iRet)
{
	//정상
	if (iRet == 0)
	{
		return 0;
	}
	if (iRet <= SOCKET_ERROR)
	{
		T_ERROR();
		return -1;
	}

	return iRet;
}
void AddUser(TUser& user)
{
	//EnterCriticalSection(&g_Crit);
	//EnterCriticalSection와 같다. 무한이 기다려준다.
	//무한대기하면 WAIT_TIMEOUT할필요가 없다. 시간을 주어줄때 WAIT_TIMEOUT을 쓴다.
	DWORD dwRet = WaitForSingleObject(g_hMutex, INFINITE);
	if (dwRet != WAIT_TIMEOUT)
	{
		g_allUser.insert(make_pair(user.sock, user));
	}
	//LeaveCriticalSection(&g_Crit);
	ReleaseMutex(g_hMutex);
}

bool BeginWinSock();
bool EndWinSock();

int SendMsg(SOCKET sock, char* msg, WORD type)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sendmsg.ph.len = strlen(msg);
	sendmsg.ph.type = type;
	memcpy(sendmsg.msg, msg, strlen(msg));

	int sendbytes = 0;
	int iTotalsize = strlen(msg) + PACKET_HEADER_SIZE;
	char* pMsg = (char*)&sendmsg;
	int iSend = 0;
	do
	{
		iSend = send(sock, (char*)&pMsg[sendbytes], iTotalsize - sendbytes, 0);
		if (CheckReturn(iSend) <= 0)
		{
			return iSend;
		}

		sendbytes += iSend;
	} while (sendbytes < iTotalsize);

	return iTotalsize;
}

int SendMsg(SOCKET sock, PACKET_HEADER ph, char* msg)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sendmsg.ph = ph;
	memcpy(sendmsg.msg, msg, strlen(msg));

	int sendbytes = 0;
	int iTotalsize = strlen(msg) + PACKET_HEADER_SIZE;
	char* pMsg = (char*)&sendmsg;
	int iSend = 0;
	do
	{
		iSend = send(sock, (char*)&pMsg[sendbytes], iTotalsize - sendbytes, 0);
		if (CheckReturn(iSend) <= 0)
		{
			return iSend;
		}

		sendbytes += iSend;
	} while (sendbytes < iTotalsize);

	return iTotalsize;
}

int SendMsg(SOCKET sock, UPACKET* uPacket)
{
	int sendbytes = 0;
	int iTotalsize = strlen(uPacket->msg) + PACKET_HEADER_SIZE;
	char* pMsg = (char*)uPacket->msg;
	int iSend = 0;
	do
	{
		iSend = send(sock, (char*)&pMsg[sendbytes], iTotalsize - sendbytes, 0);
		if (CheckReturn(iSend) <= 0)
		{
			return iSend;
		}

		sendbytes += iSend;
	} while (sendbytes < iTotalsize);

	return iTotalsize;
}
int Broadcastting(char* pMsg)
{
	//EnterCriticalSection(&g_Crit);
	DWORD dwRet = WaitForSingleObject(g_hMutex, INFINITE);

	//중요!!!:증감연산자가 없다.if문 안에서 증감한다.
	for (ITOR itor = g_allUser.begin(); itor != g_allUser.end();)
	{
		TUser* pUser = (TUser*)&(itor->second);
		if (pUser != NULL)
		{
			if (SendMsg(pUser->sock, pMsg, PACKET_CHAT_MSG) <= 0)
			{
				closesocket(pUser->sock);
				g_allUser.erase(itor++);
			}
			else
			{
				itor++;
			}
		}
	}
	ReleaseMutex(g_hMutex);
	//LeaveCriticalSection(&g_Crit);
	return 1;
}
void DelUser(TUser* pUser)
{
	printf("\n퇴장!!!!! ip = %s, port = %d\n",
		inet_ntoa(pUser->clientAddr.sin_addr), ntohs(pUser->clientAddr.sin_port));

	//EnterCriticalSection(&g_Crit);
	DWORD dwRet = WaitForSingleObject(g_hMutex, INFINITE);
	{
		ITOR itor = g_allUser.find(pUser->sock);
		if (itor != g_allUser.end())
		{
			g_allUser.erase(itor);
		}
	}
	T_ERROR();
	closesocket(pUser->sock);
	ReleaseMutex(g_hMutex);
	//LeaveCriticalSection(&g_Crit);
	return;
}

bool BeginWinSock()
{
	WSADATA wsa;

	int iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (iRet != 0)	return false;

	return true;
}
bool EndWinSock()
{
	int iRet = WSACleanup();
	if (iRet != 0)	return false;

	return true;
}
