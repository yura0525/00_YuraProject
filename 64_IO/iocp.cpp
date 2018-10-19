
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <string.h>
#include <conio.h>

#define MAX_READ_SIZE 4096
#define MAX_WORK_THREAD 4

class TIOCP
{
public:
	HANDLE m_hIOCP;
	HANDLE m_hFileRead;
	HANDLE m_hFileWrite;

	OVERLAPPED	m_hReadOV;
	OVERLAPPED	m_hWriteOV;
	HANDLE		m_hEventKillThread;

	HANDLE m_hWorkThread[MAX_WORK_THREAD];
	char m_szReadBuffer[MAX_READ_SIZE];

	bool m_bEndRead;
public:
	void Init();
	void Frame();
	static DWORD WINAPI WorkerThread(LPVOID param);
	bool WaitForRead();
	bool DispatchRead(DWORD dwTransfer);
	bool DispatchWrite(DWORD dwTransfer);

public:
	~TIOCP()
	{
		m_bEndRead = false;
	}
	void Release()
	{
		CloseHandle(m_hFileRead);
		CloseHandle(m_hFileWrite);
		CloseHandle(m_hWorkThread[0]);
		CloseHandle(m_hWorkThread[1]);
		CloseHandle(m_hWorkThread[2]);
		CloseHandle(m_hWorkThread[3]);
		CloseHandle(m_hIOCP);
	}
};

bool TIOCP::DispatchRead(DWORD dwTransfer)
{
	DWORD dwWrite;
	if (::WriteFile(m_hFileWrite, 
		m_szReadBuffer, dwTransfer, &dwWrite, 
		(LPOVERLAPPED)&m_hWriteOV) == FALSE)
	{
		//�߿�!!!:�������� ���. �񵿱� ������� ����ǰ�����.
		if (GetLastError() != ERROR_IO_PENDING)
		{
			return false;
		}
	}

	//�������� �о �� ���� ������ ������� �˷��ִ� �κ�.(m_hReadOV.Offset�� ���)
	LARGE_INTEGER data;
	data.QuadPart = dwTransfer;

	m_hReadOV.Offset += data.LowPart;
	m_hReadOV.OffsetHigh += data.HighPart;

	WaitForRead();
	return true;
}
bool TIOCP::DispatchWrite(DWORD dwTransfer)
{
	//�������� �Ἥ �� �������� ����� �κ�.
	LARGE_INTEGER data;
	data.QuadPart = dwTransfer;

	m_hWriteOV.Offset += data.LowPart;
	m_hWriteOV.OffsetHigh += data.HighPart;

	if (m_bEndRead && dwTransfer < MAX_READ_SIZE)
	{
		::SetEvent(m_hEventKillThread);
		return true;
	}

	return true;
}

DWORD WINAPI TIOCP::WorkerThread(LPVOID param)
{
	TIOCP* iocp = (TIOCP*)param;

	OVERLAPPED ov;
	DWORD bytesTransfer;
	ULONG_PTR keyValue;

	while (1)
	{
		if (WAIT_OBJECT_0 == ::WaitForSingleObject(iocp->m_hEventKillThread, 1))
		{
			break;
		}

		//�񵿱� ������� �۾������ Ȯ���ϴ� �Լ�.
		//::���� �����Լ�. 
		//::����� ���� �Լ��� �򰥸��� ����� ���Լ��� ���� Ŭ������ �ƴ϶� ������ �ִٰ� �˷��ִ� �Լ�.
		//IOCP QUEUE
		bool bRet = ::GetQueuedCompletionStatus(iocp->m_hIOCP, 
												&bytesTransfer, &keyValue, 
												(LPOVERLAPPED*)&ov, 1);

		if (bRet == TRUE)
		{
			if (bytesTransfer == 0)
			{
				::SetEvent(iocp->m_hEventKillThread);
				return true;
			}

			if (keyValue == (ULONG_PTR)iocp->m_hFileRead)
			{
				iocp->DispatchRead(bytesTransfer);
			}
			else
			{
				iocp->DispatchWrite(bytesTransfer);
			}
		}
		else
		{
			if (ERROR_HANDLE_EOF == GetLastError())
			{
				iocp->m_bEndRead = true;
				return true;
			}
			if (GetLastError() == WAIT_TIMEOUT)
			{
				continue;
			}
			//......
		}
	}
	return 0;
}
void TIOCP::Init()
{
	m_hFileRead = CreateFile(L"../../data/data_1.zip",
		GENERIC_READ, 0, 0, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
	m_hFileWrite = CreateFile(L"copy.zip",
		GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);

	//������4���� ���.
	DWORD id;
	for (int i = 0; i < MAX_WORK_THREAD; i++)
	{
		m_hWorkThread[i] = ::CreateThread(0, 0, WorkerThread, this, 0, &id);
	}

	m_hEventKillThread = ::CreateEvent(0, TRUE, FALSE, NULL);
	::ResetEvent(m_hEventKillThread);

	//����ڸ� ���.
	m_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	//����ڿ��� Read���Ͽ����� �д� ����� �Ϸ�Ǹ�, �� ����� �Ϸ�Űkey( ex)1111 )�� �������� �˷����.
	::CreateIoCompletionPort(m_hFileRead, m_hIOCP, (ULONG_PTR)m_hFileRead, 0);
	//����ڿ��� Write���Ͽ����� ���� ����� �Ϸ�Ǹ�, �� ����� �Ϸ�Űkey( ex)2222 )�� �������� �˷����.
	//�Ϸ�Ű�� �����ؾ��Ѵ�. �����ؾ��ϹǷ� �ڵ��� �ִ´�.
	::CreateIoCompletionPort(m_hFileWrite, m_hIOCP, (ULONG_PTR)m_hFileWrite, 0);
}

bool TIOCP::WaitForRead()
{
	DWORD dwReadBytes = 0;
	if (::ReadFile(m_hFileRead, m_szReadBuffer, 
		MAX_READ_SIZE, &dwReadBytes, &m_hReadOV) == FALSE)
	{
		//�߿�!!!:�������� ���. �񵿱� ������� ����ǰ�����.
		if (GetLastError() == ERROR_IO_PENDING)
		{
			return true;
		}
		return false;
	}
	//else
	//{
	//	//���� ����Ʈ���� 0����Ʈ�̰ų� ������ ��(ERROR_HANDLE_EOF)�� ���.
	//	if (dwReadBytes == 0 || GetLastError() == ERROR_HANDLE_EOF)
	//	{
	//		return false;
	//	}
	//}
	return true;
}
void TIOCP::Frame()
{
	WaitForRead();

	//��� �����尡 �ٳ��������� ��ȯ���� �ʴ´�.
	//�����尡 �Ϸ�Ȱ� �˷��ִ� �Լ�. ����Լ�.
	while (WaitForMultipleObjects(
			MAX_WORK_THREAD, 
			m_hWorkThread,
			TRUE, 1));
	Release();
}

void main()
{
	TIOCP iocp;
	iocp.Init();
	iocp.Frame();
	_getch();
}
