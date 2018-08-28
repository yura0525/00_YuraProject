#include <stdio.h>	//printf
#include <stdlib.h>	//malloc, free
#include <conio.h>	//_getche();

void main()
{
/*	printf("\n=================================\n");
	int* pPoint = (int*)malloc(sizeof(int));
	
	*pPoint = 100;
	printf("%0x번지 : %d\n", pPoint + 0, *(pPoint + 0));
	
	free(pPoint);
	
	printf("\n=================================\n");
	
	int* pPointArray = (int*)malloc(sizeof(int) * 10);
	for (int iCnt = 0; iCnt < 10; iCnt++)
	{
		pPointArray[iCnt] = iCnt * 10;
		printf("\n0x%08x번지 : %d", &(pPointArray[iCnt]), *(pPointArray + iCnt));
	}
	free(pPointArray);
	
	printf("\n=================================\n");
	
	char* pCharPoint = 0;
	int iCount = 0;
	pCharPoint = (char*)malloc(sizeof(char) * 100);

	do
	{
		pCharPoint[iCount++] = _getche();
	} while (pCharPoint[iCount - 1] != '\r');

	pCharPoint[iCount + 1] = 0;
	printf("\n0x%08x번지 : %s", pCharPoint, pCharPoint);

	for (int iCnt = 0; iCnt < iCount; iCnt++)
	{
		printf("\n0x%08X번지 : %c", pCharPoint + iCnt, *(pCharPoint + iCnt));
	}
	free(pCharPoint);
	printf("\n=================================\n");

	int* lpPoint[10];
	for (int iCnt = 0; iCnt < 10; iCnt++)
	{
		lpPoint[iCnt] = (int*)malloc(sizeof(int));
		*lpPoint[iCnt] = iCnt * 10;
		printf("\n0x%08X번지 : %d", lpPoint + iCnt, *lpPoint[iCnt]);
		//64비트에서 포인터변수의크기는 8바이트이다.
	}

	for (int iCnt = 0; iCnt < 10; iCnt++)
	{
		free(lpPoint[iCnt]);
	}
	printf("\n=================================\n");

	int iValue[3][2] = { {10,20}, {30,40}, {50,60} };
	for (int iCnt = 0; iCnt < 3; iCnt++)
	{
		printf("\n*(iValue + %d) : %p", iCnt, *(iValue + iCnt));
		printf("\t\tiValue[%d]:%p", iCnt, iValue[iCnt]);
	}
	printf("\n");

	for (int iCnt = 0; iCnt < 3; iCnt++)
	{
		for (int jCnt = 0; jCnt < 2; jCnt++)
		{
			printf("\n*(*(iValue+%d)+%d) : %d", iCnt, jCnt, *(*(iValue + iCnt) + jCnt));
			printf("\t\t *(iValue[%d] + %d) : %d", iCnt, jCnt, *(iValue[iCnt] + jCnt));
		}
	}
	*/
	printf("\n=================================\n");
	printf("\n====================이중포인터=============\n");
	system("cls");

	int* pPtr;
	int **ppPtr;
	int num = 10;

	pPtr = &num;
	ppPtr = &pPtr;
	printf("%d\n", **ppPtr);

	int **ppPoint = 0;
	//ppPoint = (int**)malloc(sizeof(int*) * 10);
	ppPoint = (int**)malloc(sizeof(intptr_t) * 10);

	printf("######## sizeof(int*) = %d, sizeof(int) = %d\n", sizeof(int*), sizeof(int));
	for (int iCnt = 0; iCnt < 10; iCnt++)
	{
		//ppPoint[iCnt] = (int*)malloc(sizeof(int) * 10);
		ppPoint[iCnt] = (int*)malloc(sizeof(intptr_t) * 10);
	}

	for (int iCnt = 0; iCnt < 10; iCnt++)
	{
		for (int jCnt = 0; jCnt < 10; jCnt++)
		{
			ppPoint[iCnt][jCnt] = iCnt * 10 + jCnt;
		}
	}

	for (int iCnt = 0; iCnt < 10; iCnt++)
	{
		printf("\n");
		for (int jCnt = 0; jCnt < 10; jCnt++)
		{
			printf("[%d,%d] = %d ", iCnt, jCnt, ppPoint[iCnt][jCnt]);
			printf("[%d,%d] = %d ", iCnt, jCnt, *(ppPoint[iCnt] + jCnt));
		}
	}
	for (int iCnt = 0; iCnt < 10; iCnt++)
	{
		free(ppPoint[iCnt]);
	}

	free(ppPoint);
	
	/*printf("\n================포인터 배열=====================\n");
	system("cls");

	char *pArray[6];
	for (int iCnt = 0; iCnt < 6; iCnt++)
	{
		pArray[iCnt] = (char*)malloc(sizeof(char) * 4);
	}
	
	for (int iCnt = 0; iCnt < 6; iCnt++)
	{
		for (int jCnt = 0; jCnt < 3; jCnt++)
		{
			pArray[iCnt][jCnt] = 65 + iCnt * 3 + jCnt;
		}
		pArray[iCnt][3] = 0;
	}

	for (int iCnt = 0; iCnt < 6; iCnt++)
	{
		printf("\n");
		printf("[%d] = %s", iCnt, pArray[iCnt]);
	}

	for (int iCnt = 0; iCnt < 6; iCnt++)
	{
		free(pArray[iCnt]);
	}*/

	getchar();
}