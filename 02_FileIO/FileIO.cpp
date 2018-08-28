#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <windows.h>

void DrawText()
{
	FILE * fp = fopen("FileIO.cpp", "r");
	char buffer[255] = { 0, };
	int iLine = 0;
	while (!feof(fp))
	{
		printf("\n%05d : ", ftell(fp));
		for (int i = 0; i < 10; i++)
		{
			buffer[i] = fgetc(fp);

			if (feof(fp)) break;
		}

		for (int i = 0; i < 10; i++)
		{
			if (buffer[i] == '\n' || buffer[i] == '\t' || buffer[i] == '\r')
			{
				buffer[i] = '.';
			}
			printf("%c", buffer[i]);
		}
		iLine++;
		if (iLine % 10 == 0)
		{
			printf("\n아무키나 누르시오");
			_getch();
		}
		//fgets(buffer, sizeof(char) * 255, fp);
		//printf("\n%s", buffer);
	}
	fclose(fp);
}

void fileCopy(const char * srcFile, const char* destFile)
{
	FILE * readfp = fopen(srcFile, "r");
	FILE * writefp = fopen(destFile, "w");
	
	fseek(readfp, 0, SEEK_END);
	
	int iTotalSize = ftell(readfp);		//읽은 바이트수
	
	//char* buffer = (char*)calloc(iTotalSize, sizeof(int));
	char* buffer = (char*)malloc(iTotalSize);
	
	memset(buffer, 0, iTotalSize);		//버퍼를 0으로 초기화
	fseek(readfp, 0, SEEK_SET);

	//fread(buffer, sizeof(char), iTotalSize, readfp);
	//fwrite(buffer, sizeof(char), iTotalSize, writefp);
	fread(buffer, 1, sizeof(char) * iTotalSize, readfp);
	fwrite(buffer, 1, sizeof(char) * iTotalSize, writefp);
	//fread(buffer, sizeof(char) * iTotalSize, 1, readfp);
	//fwrite(buffer, sizeof(char) * iTotalSize, 1, writefp);
	fclose(readfp);
	fclose(writefp);
}
void main()
{
	//DrawText();
	system("cls");

	fileCopy("FileIO.cpp", "copy.cpp");
	getchar();
}