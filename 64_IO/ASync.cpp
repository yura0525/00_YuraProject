
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <string.h>
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
	printf("파일 복사 완료");
	delete[] buf;
	CloseHandle(hWriteFile);
}
