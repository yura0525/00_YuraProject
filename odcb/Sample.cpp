#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>

SQLHENV      g_hEnv;
SQLHDBC      g_hDbc;
SQLHSTMT   g_hStmt;

void Select();
//TCHAR szSQL[] = _T("Insert into tblCigar(name, price, korean) values('bbb', 3000, 1)");

bool Add()
{
	TCHAR szSQL[256] = { 0, };
	SQLTCHAR Name[21] = _T("금연");
	SQLINTEGER Price = 3000;
	BOOL Korean = FALSE;

	_stprintf((TCHAR*)szSQL, _T("Insert into tblCigar(name, price, korean) values('%s', %d, %d)"), Name, Price, Korean);
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)szSQL, SQL_NTS) == SQL_SUCCESS)
	{
		//조회할때 커서의 위치로부터 밑으로 조회한다.
		//그래서 커서를 닫아서 다시 검색 할때 맨위로 올려야한다.
		SQLCloseCursor(g_hStmt);
		Select();
		return true;
	}
	SQLCloseCursor(g_hStmt);
	return false;
}

//select..from table
//select..from table where ?=?;

void Select()
{
	SQLTCHAR Name[20] = { 0, };
	int iPrice, isKorean;
	SQLLEN lName, lPrice, lKorean;
	//SQL_C_CHAR 짧은 텍스트 SQL_C_ULONG 숫자.
	//1번 필드 2번 필드.
	SQLBindCol(g_hStmt, 1, SQL_C_CHAR, Name, sizeof(Name), &lName);
	SQLBindCol(g_hStmt, 2, SQL_C_ULONG, &iPrice, sizeof(iPrice), &lPrice);
	SQLBindCol(g_hStmt, 3, SQL_C_ULONG, &isKorean, sizeof(isKorean), &lKorean);

	//tblCigar테이블로부터 name(필드명)을 리턴해라.
	//select...from...where... '문자열' 
	//if (SQLExecDirect(g_hStmt, (SQLTCHAR*)_T("select name,price,korean from tblCigar where name='This'"), SQL_NTS) != SQL_SUCCESS)
	//{
	//   return;
	//}
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)_T("select * from tblCigar"), SQL_NTS) != SQL_SUCCESS)
	{
		return;
	}

	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		printf("\n%s : price[%d][%d]", Name, iPrice, isKorean);
	}

	//조회할때 커서의 위치로부터 밑으로 조회한다.
	//그래서 커서를 닫아서 다시 검색 할때 맨위로 올려야한다.
	SQLCloseCursor(g_hStmt);
}

bool Delete()
{
	TCHAR szSQL[256] = _T("Delete from tblCigar where name = '금연'");
	SQLTCHAR Name[21] = _T("금연");

	//_stprintf((TCHAR*)szSQL, _T("Delete from tblCigar where name = '%s'"), Name);
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)szSQL, SQL_NTS) == SQL_SUCCESS)
	{
		//조회할때 커서의 위치로부터 밑으로 조회한다.
		//그래서 커서를 닫아서 다시 검색 할때 맨위로 올려야한다.
		SQLCloseCursor(g_hStmt);
		Select();
		return true;
	}
	SQLCloseCursor(g_hStmt);
	return false;
}

bool Update()
{
	TCHAR szSQL[256] = _T("Update tblCigar set name = '금연자' where name = '금연'");
	SQLTCHAR Name[21] = _T("금연");
	SQLINTEGER Price = 3000;
	BOOL Korean = FALSE;

	//_stprintf((TCHAR*)szSQL, _T("Update tblCigar where name = '%s'"), Name);
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)szSQL, SQL_NTS) == SQL_SUCCESS)
	{
		//조회할때 커서의 위치로부터 밑으로 조회한다.
		//그래서 커서를 닫아서 다시 검색 할때 맨위로 올려야한다.
		SQLCloseCursor(g_hStmt);
		Select();
		return true;
	}
	SQLCloseCursor(g_hStmt);
	return false;
}

void main()
{
	//환경 핸들
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &g_hEnv) != SQL_SUCCESS)
		return;

	if (SQLSetEnvAttr(g_hEnv, SQL_ATTR_ODBC_VERSION,
		(SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
		return;

	//연결 핸들
	//환경핸들을 먼저 만들고 환경핸들을 통해서 연결핸들을 생성
	//환경핸들은 부모가 NULL이고 연결핸들 부모는 환경핸들이다.

	SQLAllocHandle(SQL_HANDLE_DBC, g_hEnv, &g_hDbc);

	// 주의 --> *.mdb,"한칸뛰어야 한다."*.accdb
	SQLTCHAR szInCon[256] = { 0, };
	_stprintf((TCHAR*)szInCon,
		_T("DRIVER={Microsoft Access Driver (*.mdb)};DBQ=%s\\cigarette.mdb;"), L"..\\..\\data");

	SQLTCHAR szOutcon[256] = { 0, };
	SQLSMALLINT cbCon;
	SQLRETURN ret = SQLDriverConnect(g_hDbc, NULL,
		szInCon, _countof(szInCon), szOutcon, _countof(szOutcon),
		&cbCon, SQL_DRIVER_NOPROMPT);

	if (ret != SQL_SUCCESS && (ret != SQL_SUCCESS_WITH_INFO))
		return;

	//명령 핸들
	SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hStmt);

	while (1)
	{
		int iSelect;
		printf("\n0:출력, 1:추가, 2:삭제, 3: 수정\n");
		scanf_s("%d", &iSelect);
		switch (iSelect)
		{
		case 0: Select();   break;
		case 1:   Add();      break;
		case 2:   Delete();   break;
		case 3:   Update();    break;
		}
	}

	SQLDisconnect(g_hDbc);

	SQLFreeHandle(SQL_HANDLE_STMT, g_hStmt);
	SQLFreeHandle(SQL_HANDLE_DBC, g_hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, g_hEnv);
}