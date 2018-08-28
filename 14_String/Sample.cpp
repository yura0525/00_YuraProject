#include <string>
#include <iostream>
#include <tchar.h>

using namespace std;
void printer(const char* pData)
{
	cout << pData;
}
int main()
{
	char data[] = "kgca";
	char dataA[] = "game";
	string copyData;
	string copyDataA = data;

	//wchar_t data[] = L"kgca";
	//wchar_t dataA[] = L"game";
	//wstring copyData;
	//wstring copyDataA = data;

	/*TCHAR data[] = _T("kgca");
	TCHAR dataA[] = _T("game");
	wstring copyData;
	wstring copyDataA = data;*/


	copyData = data;
	copyData += dataA;
	cout << copyData;

	if (copyData == copyDataA)
	{
	}
	printer(copyData.c_str());
	return 0;
}