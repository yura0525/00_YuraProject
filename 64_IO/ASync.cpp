
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <string.h>
//�����ڵ忡�� ��Ƽ����Ʈ�� �ٲٴ� �Լ�.
char* GetW2M(WCHAR* data)
{
	char cData[3333] = { 0, };

	//��ȯ�� ũ�⸦ ã�´�.
	int iLength = WideCharToMultiByte(CP_ACP, 0, data, -1, 0, 0, NULL, NULL);

	int iRet = WideCharToMultiByte(CP_ACP, 0, data, -1, cData, iLength, NULL, NULL);
	return cData;
}

//��Ƽ����Ʈ���� �����ڵ�� �ٲٴ� �Լ�.
WCHAR* GetM2W(char* data)
{
	WCHAR cData[3333] = { 0, };

	//��ȯ�� ũ�⸦ ã�´�.
	int iLength = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);

	int iRet = MultiByteToWideChar(CP_ACP, 0, data, -1, cData, iLength);
	return cData;
}

std::string w2m(std::wstring data)
{
	std::string ret = std::string(data.begin(), data.end());
	return ret;
}

std::wstring m2w(std::string data)
{
	std::wstring ret = std::wstring(data.begin(), data.end());
	return ret;
}

void main()
{
	char* buf = NULL;
	LARGE_INTEGER filesize;
	DWORD dwLength;

	OVERLAPPED ov;
	ZeroMemory(&ov, sizeof(OVERLAPPED));
	//wov.hEvent = CreateEvent();


	HANDLE hReadFile = CreateFileA("../../data/Dance The Night Away.flac", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (hReadFile != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(hReadFile, &filesize);

		buf = new char[filesize.QuadPart];
		dwLength = filesize.QuadPart;
		DWORD dwRead;

		bool iRet = ReadFile(hReadFile, buf, dwLength, 
			&dwRead, &ov);

		//�񵿱�������Լ�. OVERLAPPED�� ����ߴ�.
		//�׷��� �бⰡ ������ ���� ���ϵǾ iRet == FALSE�̹Ƿ�
		//GetLastError() == ERROR_IO_PENDING���� �а��ִ� ������ Ȯ���ؾ��Ѵ�.
		//�бⰡ �� �������� ������ ������Ѵ�.
		if (iRet == FALSE)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				printf("\n%s", "ERROR_IO_PENDING Read...");
			}
		}
		
		while (1)
		{
			DWORD trans;
			bool ret = GetOverlappedResult(hReadFile, &ov, &trans, FALSE);
			if (ret == TRUE)
			{
				printf("\n%s", "finishRead...");
				break;
			}
			else
			{
				printf("\n%s", "Read...");
			}
		}
	}
	CloseHandle(hReadFile);

	
	HANDLE hWriteFile = CreateFileA("copy.flac", 
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (hWriteFile != NULL)
	{
		DWORD dwWritten;
		bool iRet = WriteFile(hWriteFile, buf, dwLength, 
								&dwWritten, &ov);

		if (iRet == FALSE)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				printf("\n%s", "ERROR_IO_PENDING Write...");
			}
		}

		while (1)
		{
			DWORD dwRet = WaitForSingleObject(hReadFile, 0);
			
			if (dwRet == WAIT_OBJECT_0)
			{
				printf("\n%s : %ld", "FinishWrite...", ov.InternalHigh);
				break;
			}
			else if (dwRet == WAIT_TIMEOUT)
			{
				DWORD trans;
				bool ret = GetOverlappedResult(hReadFile, &ov, &trans, FALSE);
				if (ret == TRUE)
				{
					printf("\n%s : %ld", "FinishWrite...", ov.InternalHigh);
					break;
				}
				else
				{
					static DWORD dwSum = 0;
					dwSum += ov.InternalHigh;
					printf("\n%s : %ld: %ld", "Write...", ov.InternalHigh, dwSum);
				}
			}	
		}
	}
	printf("���� ���� �Ϸ�");
	delete[] buf;
	CloseHandle(hWriteFile);
}
