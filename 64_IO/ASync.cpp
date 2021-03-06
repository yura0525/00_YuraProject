/*
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <string.h>

OVERLAPPED ov;
LARGE_INTEGER	g_size;
char* g_buf = NULL;

//유니코드에서 멀티바이트로 바꾸는 함수.
char* GetW2M(WCHAR* data)
{
	char cData[3333] = { 0, };

	//변환된 크기를 찾는다.
	int iLength = WideCharToMultiByte(CP_ACP, 0, data, -1, 0, 0, NULL, NULL);

	int iRet = WideCharToMultiByte(CP_ACP, 0, data, -1, cData, iLength, NULL, NULL);
	return cData;
}

//멀티바이트에서 유니코드로 바꾸는 함수.
WCHAR* GetM2W(char* data)
{
	WCHAR cData[3333] = { 0, };

	//변환된 크기를 찾는다.
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

//4096만큼 쪼개서 읽는다.
//파일을 한번에 읽으면 부담스럽기 때문에 쪼개서 읽는다.
void FileRead(HANDLE hReadFile, DWORD offset)
{
	DWORD dwLength = 4096;			//dwLength = filesize.QuadPart;
	g_size.QuadPart += offset;

	ov.Offset = g_size.LowPart;
	ov.OffsetHigh = g_size.HighPart;
	char* pTemp = &g_buf[g_size.QuadPart];

	DWORD dwRead;
	bool iRet = ReadFile(hReadFile, pTemp, dwLength, &dwRead, &ov);

	//비동기입출력함수. OVERLAPPED을 사용했다.
	//그래서 읽기가 끝나기 전에 리턴되어서 iRet == FALSE이므로
	//GetLastError() == ERROR_IO_PENDING으로 읽고있는 중인지 확인해야한다.
	//읽기가 다 끝났을때 통지를 해줘야한다.
	if (iRet == FALSE)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			printf("\n%s", "ERROR_IO_PENDING Read...");
		}	
	}
}
void main()
{
	LARGE_INTEGER filesize;

	ZeroMemory(&ov, sizeof(OVERLAPPED));
	//wov.hEvent = CreateEvent();

	HANDLE hReadFile = CreateFileA(
									"../../data.zip", 
									GENERIC_READ, 
									0, 
									NULL,
									OPEN_EXISTING, 
									FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
									NULL);
	if (hReadFile != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(hReadFile, &filesize);

		g_buf = new char[filesize.QuadPart];
		if (g_buf == NULL)
		{
			return;
		}
		FileRead(hReadFile, 0);
	}

	while (1)
	{
		DWORD trans = 0;
		bool ret = GetOverlappedResult(hReadFile, &ov, &trans, TRUE);
		if (ret == TRUE)
		{
			if (filesize.QuadPart <= g_size.QuadPart)
			{
				break;
			}
			FileRead(hReadFile, trans);
			printf("\n%s : %ld", "FinishRead...", g_size.QuadPart );
		}
		else
		{
			printf("\n%s", "READ...");
		}
	}	
	CloseHandle(hReadFile);

	OVERLAPPED wov;
	ZeroMemory(&wov, sizeof(OVERLAPPED));
	HANDLE hWriteFile = CreateFileA("data_1.zip", 
									GENERIC_READ | GENERIC_WRITE, 0, NULL,
									CREATE_ALWAYS,
									FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
									NULL);
	if (hWriteFile != NULL)
	{
		DWORD dwWritten;
		bool iRet = WriteFile(hWriteFile, g_buf, filesize.QuadPart, 
								&dwWritten, &wov);

		if (iRet == FALSE)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				printf("\n%s", "ERROR_IO_PENDING Write...");
			}
		}

		while (1)
		{
			DWORD dwRet = WaitForSingleObject(hWriteFile, 0);

			if (dwRet == WAIT_OBJECT_0)
			{
				printf("\n%s : %ld", "FinishWrite...", wov.InternalHigh);
				break;
			}
			else if (dwRet == WAIT_TIMEOUT)
			{
				DWORD trans = 0;
				bool ret = GetOverlappedResult(hWriteFile, &wov, &trans, FALSE);
				if (ret == TRUE)
				{
					printf("\n%s : %ld", "FinishWrite...", wov.InternalHigh);
					break;
				}
				else
				{
					static DWORD dwSum = 0;
					dwSum += wov.InternalHigh;
					printf("\n%s : %ld: %ld", "Write...", wov.InternalHigh, dwSum);
				}
			}
		}
	}
	CloseHandle(hWriteFile);
	printf("파일 복사 완료");
	delete[] g_buf;
}
*/
