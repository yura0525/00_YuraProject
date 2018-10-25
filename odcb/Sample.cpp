#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>

void main()
{
	//환경 핸들
	SQLHENV		g_hEnv;
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &g_hEnv) != SQL_SUCCESS )
	{
		return;
	}
	
	if (SQLSetEnvAttr(g_hEnv, SQL_ATTR_ODBC_VERSION,
		(SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
	{
		return;
	}

	//연결 핸들
	//환경핸들을 먼저 만들고 환경핸들을 통해서 연결핸들을 생성
	//환경핸들은 부모가 NULL이고 연결핸들 부모는 환경핸들이다.
	SQLHDBC		g_hDbc;
	SQLAllocHandle(SQL_HANDLE_DBC, g_hEnv, &g_hDbc);
	
	// 주의 --> *.mdb,"한칸뛰어야 한다."*.accdb
	SQLTCHAR szInCon[256] = { 0, };
	_stprintf(szInCon, 
		_T("DRIVER={Microsoft Access Driver (*.mdb)};DBQ=%s\\cigarette.mdb;"), L"..\\..\\data");

	SQLTCHAR szOutcon[256] = { 0, };
	SQLSMALLINT cbCon;
	SQLRETURN ret = SQLDriverConnect(g_hDbc, NULL,
		szInCon, _countof(szInCon), szOutcon, _countof(szOutcon), 
		&cbCon, SQL_DRIVER_NOPROMPT);

	if (ret != SQL_SUCCESS && (ret != SQL_SUCCESS_WITH_INFO))
	{
		return;
	}
	//명령 핸들
	SQLHSTMT	g_hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hStmt);

	
	SQLCHAR Name[20] = { 0, };
	int iPrice, isKorean;
	SQLLEN lName, lPrice, lKorean;
	//SQL_C_CHAR 짧은 텍스트 SQL_C_ULONG 숫자.
	//1번 필드 2번 필드.
	SQLBindCol(g_hStmt, 1, SQL_C_CHAR, Name, sizeof(Name), &lName);
	SQLBindCol(g_hStmt, 2, SQL_C_ULONG, &iPrice, sizeof(iPrice), &lPrice);
	SQLBindCol(g_hStmt, 3, SQL_C_ULONG, &isKorean, sizeof(isKorean), &lKorean);

	//tblCigar테이블로부터 name(필드명)을 리턴해라.
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)_T("select name,price,korean from tblCigar"), SQL_NTS) != SQL_SUCCESS)
	{
		return;
	}

	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		printf("\n%s : price[%d][%d]", Name, iPrice, isKorean);
	}

	SQLCloseCursor(g_hStmt);

	SQLDisconnect(g_hDbc);
	
	SQLFreeHandle(SQL_HANDLE_STMT, g_hStmt);
	SQLFreeHandle(SQL_HANDLE_DBC, g_hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, g_hEnv);
}