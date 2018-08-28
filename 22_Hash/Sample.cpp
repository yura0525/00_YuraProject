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
	//�Ϲ������� �ּҰ��� ���� ����ũ�ϱ� ������, �ؽô� �ּҰ����� ���� ����Ѵ�.
	//�浹�� ���Ҽ��� ������, �л��� ���� �ȴ�.

	//�ּҰ��� 4����Ʈ�ΰ�
	//0000 0000 0000 xxxx
	//& 0xff �ص忬������ 1�� ä���.
	//�ڿ� 1����Ʈ�� ����Ѵ�.
	//�ڿ� 1����Ʈ�� ����Ѵ�.
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

//16������ �ǹ� ���� ������.
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
	//�ؽ� �Լ��� �л��� ���ϴ��� ��ġ�� �浹�� �Ұ����ϴ�.
	//��� ������ ������ ���ؼ��� �ؽÿ� �Բ� ���Ḯ��Ʈ�� ����ؾ� 
	//��絥���͸� �浹���� �����Ҽ� �ִ�.

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