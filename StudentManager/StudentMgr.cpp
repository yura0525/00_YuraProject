#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <io.h>		//_file관련 함수
#define _CRT_SECURE_NO_WARNINGS

#define MAX_COUNT 30

enum eWork
{
	eCreate = 0,
	ePrint,
	eFindName,
	eModifyIndex,
	eSaveFile,
	eLoadFile,
	eQuit = 99,
};
class Student
{
public :
	int		m_idx;
	char	m_name[4];
	int		m_kor;
	int		m_eng;
	int		m_math;
	int		m_total;
	float	m_average;

//public:
//	void PrintStudent(int idx);
//	void FindStudent(char* findName);
//	void PrintStudentAll();
//	void SaveFile();
//	void LoadFile(Student * studentArr);
//	void FindFile();
//	void SortFile(_finddata_t * file);
};

int g_StudentSize = 0;
Student* g_pStudetnArr = NULL;

//파일검색및 저장 배열
struct _finddata_t	g_SaveFileArray[MAX_COUNT];
int					g_iNumSaveFile = 0;

void PrintStudent(int idx);
void FindStudent(char* findName);
void PrintStudentAll();
void SaveFile();
void LoadFile(Student * studentArr);
void FindFile();
void SortFile(_finddata_t * file);

//함수포인터
void (*Work)();
void Excute(void(*process)())
{
	process();
}
void main()
{
	Student studentArr[30];
	char*	findName;

	int		workIdx = 0;
	int		modifyIdx = 0;

	srand(time(NULL));

	while (1)
	{
		printf("\n원하는 작업을 출력하시오 : \n신규(0), 출력(1), 이름으로 검색(2), 수정(3), 파일저장(4), 파일로드(5), 종료(99)\n");
		scanf_s("%d", &workIdx);

		if (workIdx == eQuit)
		{
			while (getchar() != '\n');
			printf("정말로 끝내시겠습니까?\n");
			getchar();
			break;
		}

		switch (workIdx)
		{
		case eCreate:
		{
			for (int idx = 0; idx < MAX_COUNT; idx++)
			{
				studentArr[idx].m_idx = idx;
				for (int j = 0; j < 3; j++)
				{
					studentArr[idx].m_name[j] = (rand() % 26) + 'A';
				}
				studentArr[idx].m_name[3] = '\0';

				studentArr[idx].m_kor = rand() % 101;
				studentArr[idx].m_eng = rand() % 101;
				studentArr[idx].m_math = rand() % 101;

				studentArr[idx].m_total = studentArr[idx].m_kor
					+ studentArr[idx].m_eng
					+ studentArr[idx].m_math;

				studentArr[idx].m_average = studentArr[idx].m_total / 3.0f;
			}
			g_StudentSize = MAX_COUNT;
			g_pStudetnArr = studentArr;
			break;
		}
		case ePrint:
		{
			Excute(PrintStudentAll);
			//PrintStudentAll();
			break;
		}
		case eFindName:
		{
			printf("찾으실 이름을 입력하세요:");
			//while (getchar() != '\n');

			findName = (char*)malloc(sizeof(char) * 4);
			scanf_s("%s", findName);

			FindStudent(findName);

			free(findName);
			break;
		}
		case eModifyIndex:
		{
			printf("수정할 인덱스 :");
			//while (getchar() != '\n');

			scanf_s("%d", &modifyIdx);
			if ((modifyIdx < 0) || (g_StudentSize <= modifyIdx))
			{
				printf("수정할 인덱스가 배열범위를 벗어났습니다.\n");
				break;
			}
			else
			{
				int modifyKor, modifyEng, modifyMath;
				printf("이름\t 국어\t 영어\t 수학\n");

				findName = (char*)malloc(sizeof(char) * 4);
				scanf_s("%s", findName);
				scanf_s("%d", &modifyKor);
				scanf_s("%d", &modifyEng);
				scanf_s("%d", &modifyMath);

				strcpy(studentArr[modifyIdx].m_name, findName);
				studentArr[modifyIdx].m_name[3] = '\0';

				studentArr[modifyIdx].m_kor = modifyKor;
				studentArr[modifyIdx].m_eng = modifyEng;
				studentArr[modifyIdx].m_math = modifyMath;

				studentArr[modifyIdx].m_total = studentArr[modifyIdx].m_kor
					+ studentArr[modifyIdx].m_eng
					+ studentArr[modifyIdx].m_math;

				studentArr[modifyIdx].m_average = studentArr[modifyIdx].m_total / 3.0f;

				PrintStudentAll();
				free(findName);
			}
			break;
		}
		case eSaveFile:
		{
			Excute(SaveFile);
			//SaveFile();
			break;
		}
		case eLoadFile:
		{
			LoadFile(studentArr);
			break;
		}
		}
	}
}

void FindStudent(char* findName)
{
	int	findNameIdx = -1;

	for (int idx = 0; idx < g_StudentSize; idx++)
	{
		if (!_stricmp(g_pStudetnArr[idx].m_name, findName))
		{
			findNameIdx = idx;
			break;
		}
	}

	if (findNameIdx != -1 && findNameIdx < g_StudentSize)
	{
		PrintStudent(findNameIdx);
	}
	else
	{
		printf("해당하는 이름을 찾을수 없습니다\n");
	}
}
void PrintStudent(int idx)
{
	if (g_pStudetnArr == 0)
	{
		printf("데이터가 비어있습니다. 먼저 생성하세요.\n");
		return;
	}
	printf("%d\t %s\t %d\t %d\t %d\t %d\t %f\n",
		idx, g_pStudetnArr[idx].m_name, g_pStudetnArr[idx].m_kor, g_pStudetnArr[idx].m_eng, g_pStudetnArr[idx].m_math,
		g_pStudetnArr[idx].m_total, g_pStudetnArr[idx].m_average);
}

void PrintStudentAll()
{
	printf("번호\t 이름\t 국어\t 영어\t 수학\t 총점\t 평균\n");
	for (int idx = 0; idx < g_StudentSize; idx++)
	{
		PrintStudent(idx);
	}
}

void SaveFile()
{
	time_t now			= time(NULL);
	tm* lt				= localtime(&now);
	char fileName[256]	= { 0, };

	strftime(fileName, 256, "%Y년%m월%d일%H시%M분%S초.txt", lt);

	FILE* fp = fopen(fileName, "w");
	if (fp == NULL)
	{
		printf("%s 파일이 생성되지 않았습니다.\n", fileName);
		return;
	}

	fprintf(fp, "%d\n", g_StudentSize);
	//fprintf(fp, "번호\t 이름\t 국어\t 영어\t 수학\t 총점\t 평균\n");
	for (int idx = 0; idx < g_StudentSize; idx++)
	{
		fprintf( fp, "%d\t %s\t %d\t %d\t %d\t %d\t %f\n",
			idx, g_pStudetnArr[idx].m_name, g_pStudetnArr[idx].m_kor, g_pStudetnArr[idx].m_eng, g_pStudetnArr[idx].m_math,
			g_pStudetnArr[idx].m_total, g_pStudetnArr[idx].m_average);
	}
	fclose(fp);
}

void FindFile()
{
	struct _finddata_t file;
	intptr_t hFile = _findfirst("*.txt", &file);

	struct _finddata_t next;
	if (hFile == -1L)
	{
		printf("파일이 존재하지 않습니다.\n");
		return;
	}
	else
	{
		printf("파일 목록들\n");
		printf("\nNOR RDO HID SYS ARC SUB dir FILE DATE SIZE\n");
		printf("============================================\n");
		next = file;
		do {
			char buffer[30];
			printf((file.attrib & _A_NORMAL) ? " Y " : " N ");
			printf((file.attrib & _A_RDONLY) ? " Y " : " N ");
			printf((file.attrib & _A_SYSTEM) ? " Y " : " N ");
			printf((file.attrib & _A_HIDDEN) ? " Y " : " N ");
			printf((file.attrib & _A_ARCH) ? " Y " : " N ");
			printf((file.attrib & _A_SUBDIR) ? " Y " : " N ");

			ctime_s(buffer, _countof(buffer), &file.time_write);

			printf(" %s %s %d\n", file.name, ctime(&(file.time_write)), file.size);

			SortFile(&file);

		} while (_findnext(hFile, &file) == 0);

		_findclose(hFile);
	}
	//1)특정폴더의 모든 파일을 검색한다.
	//2)검색된 파일 중에 최근파일을 반환한다.
	//3)검색된 파일중에 원하는 파일을 로드한다.
}


void LoadFile(Student * studentArr)
{
	int inputArraySize = 0;
	int idx, kor, eng, math, total;
	float average = 0.0f;
	char* name = (char*)malloc(sizeof(char) * 4);

	FindFile();
	if (g_iNumSaveFile < 1)	return;

	FILE* fp = fopen(g_SaveFileArray[g_iNumSaveFile-1].name, "r");
	if (fp == NULL)
	{
		printf("%s 파일이 없습니다.\n", g_SaveFileArray[g_iNumSaveFile - 1].name);
		return;
	}

	fscanf(fp, "%d", &inputArraySize);
	if (MAX_COUNT < inputArraySize)
	{
		printf("저장하려는 데이터가 배열보다 큽니다.\n입력된 사이즈 = %d, 배열의 크기 = %d\n", inputArraySize, MAX_COUNT);
		return;
	}
	
	g_pStudetnArr = studentArr;
	g_StudentSize = inputArraySize;

	for (int i = 0; i < inputArraySize; i++)
	{
		fscanf(fp, "%d %s %d %d %d %d %f", &idx, name, &kor, &eng, &math, &total, &average);

		g_pStudetnArr[i].m_idx		= idx;
		strcpy(g_pStudetnArr[i].m_name, name);
		g_pStudetnArr[i].m_kor		= kor;
		g_pStudetnArr[i].m_eng		= eng;
		g_pStudetnArr[i].m_math		= math;
		g_pStudetnArr[i].m_total	= total;
		g_pStudetnArr[i].m_average	= average;
	}

	printf("로드 되어있는 데이터를 출력합니다.\n");
	PrintStudentAll();

	fclose(fp);
}


//최근 생성한 일자를 기준으로 앞에서부터 정렬된다.
void SortFile(_finddata_t * pFile)
{
	//최대 30개만 저장한다.
	if (g_iNumSaveFile >= MAX_COUNT)
		return;

	_finddata_t file;
	int iIndex = 0;
	for (int iIndex = 0; iIndex < g_iNumSaveFile; iIndex++)
	{
		memcpy(&file, &g_SaveFileArray[iIndex], sizeof(_finddata_t));

		if (file.time_create < pFile->time_create)
			break;
	}
	for (int iCnt = g_iNumSaveFile; iCnt > iIndex; iCnt--)
	{
		memcpy(&g_SaveFileArray[iCnt], &g_SaveFileArray[iCnt - 1], sizeof(_finddata_t));
	}
	memcpy(&g_SaveFileArray[iIndex], pFile, sizeof(_finddata_t));

	g_iNumSaveFile++;
}
