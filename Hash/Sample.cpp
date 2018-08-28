#include <iostream>
#include <string>
using namespace std;

#define TABLE_SIZE 200

static const char* const testCases[] = {
"this",
"is",
"a",
"kgca",
"test",
"but",
"i",
"should",
"use",
"a",
"real",
"dictionary"
};

char* testCases1[100];
unsigned int HashCast(const char* cp)
{
	unsigned int ret = reinterpret_cast<unsigned int>(cp);
	//일반적으로 주소값이 거의 유니크하기 때문에, 해시는 주소값으로 많이 사용한다.
	//충돌을 피할수는 없지만, 분산이 골고루 된다.

	//주소값이 4바이트인걸
	//0000 0000 0000 xxxx
	//& 0xff 앤드연산으로 1로 채우면.
	//뒤에 1바이트만 사용한다.
	//뒤에 1바이트만 사용한다.
	unsigned byteRet1 = (ret >> 0) & 0xff;
	unsigned byteRet2 = (ret >> 8) & 0xff;
	//0000 0000 0000 xxxx
	//&
	//				 ffff
	return ((byteRet1 + byteRet2) % TABLE_SIZE);
}
unsigned int Hash(const char* source)
{
	unsigned int key = 0;
	while (*source)
	{
		key += *source++;
	}
	return (key % TABLE_SIZE);
}

//16진수는 의미 없는 숫자임.
unsigned int Hash2(const char* source)
{
	unsigned int key = 0x711c9dc5;
	while (*source)
	{
		key ^= *source++;
		key *= 0x711c9dc5;
	}
	return (key % TABLE_SIZE);
}

struct TTable
{
	const char* table[10];
	int iCount;
	void Add(const char *pData)
	{
		if ( iCount >= 9)
			return;

		table[iCount++] = pData;
	}

	const char* Get(const char* pData)
	{
		if( iCount == 1)
			return table[0];

		for (int i = 0; i < iCount; i++)
		{
			if ( !strcmp(table[i], pData) )
			{
				return table[i];
			}
		}
		return NULL;
	}

	TTable()
	{
		iCount = 0;
	}
};
int main()
{
	//해시 함수로 분산을 잘하더라도 겹치는 충돌은 불가피하다.
	//모든 데이터 저장을 위해서는 해시와 함께 연결리스트를 사용해야 
	//모든데이터를 충돌없이 저장할수 있다.

	//////example 1
	//const char* table[TABLE_SIZE] = { 0, };
	//int iNumCount = sizeof(testCases) / sizeof(testCases[0]);

	//for (int i = 0; i < iNumCount; i++)
	//{
	//	const char * source = testCases[i];
	//	//unsigned int iKey = Hash(source);
	//	unsigned int iKey = Hash2(source);

	//	table[iKey] = source;
	//	//std::cout << iKey << std::endl;
	//}

	//for (int i = 0; i < iNumCount; i++)
	//{
	//	const char * source = testCases[i];
	//	unsigned int iKey = Hash2(source);

	//	//table[iKey] = source;
	//	std::cout << "iKey  = " << iKey << " Value = " << table[iKey] << std::endl;
	//}

	//////example 2
	//TTable table[TABLE_SIZE] = {};
	//int iNumCount = sizeof(testCases) / sizeof(testCases[0]);

	//for (int i = 0; i < iNumCount; i++)
	//{
	//	const char * source = testCases[i];
	//	//unsigned int iKey = Hash(source);
	//	unsigned int iKey = HashCast(source);

	//	table[iKey].Add(source);

	//	//table[iKey] = source;
	//	//std::cout << iKey << std::endl;
	//}

	//for (int i = 0; i < iNumCount; i++)
	//{
	//	const char * source = testCases[i];
	//	unsigned int iKey = HashCast(source);

	//	std::cout << "iKey  = " << iKey << " Value = " << table[iKey].Get(source) << std::endl;
	//}

	//////example 3
	for (int iCnt = 0; iCnt < 100; iCnt++)
	{
		testCases1[iCnt] = new char[4];
		testCases1[iCnt][0] = 65 + (rand() % 26);
		testCases1[iCnt][1] = 65 + (rand() % 26);
		testCases1[iCnt][2] = 65 + (rand() % 26);

		testCases1[iCnt][3] = 0;
	}

	TTable table[TABLE_SIZE] = {};
	//int iNumCount = sizeof(testCases) / sizeof(testCases[0]);

	for (int i = 0; i < 100; i++)
	{
		const char * source = testCases1[i];
		//unsigned int iKey = Hash(source);
		unsigned int iKey = HashCast(source);

		table[iKey].Add(source);

		//table[iKey] = source;
		//std::cout << iKey << std::endl;
	}

	for (int i = 0; i < 100; i++)
	{
		const char * source = testCases1[i];
		unsigned int iKey = HashCast(source);

		std::cout << "iKey  = " << iKey << " Value = " << table[iKey].Get(source) << std::endl;
	}
	return 0;
}