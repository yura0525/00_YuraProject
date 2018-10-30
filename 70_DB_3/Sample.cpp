#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>

SQLHENV		g_hEnv;
SQLHDBC		g_hDbc;
SQLHSTMT	g_hStmt;

void Select();
//TCHAR szSQL[] = _T("Insert into tblCigar(name, price, korean) values('bbb', 3000, 1)");

SQLRETURN HandleDiagnosticRecord()
{
	int ii;
	SQLRETURN Ret;
	SQLINTEGER NativeError;
	SQLTCHAR SqlState[6], Msg[255];
	SQLSMALLINT MsgLen;
	TCHAR str[256];

	ii = 1;
	while (Ret = SQLGetDiagRec(SQL_HANDLE_STMT, g_hStmt, ii, SqlState, &NativeError,
		Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA)
	{
		wsprintf(str, _T("SQLSTATE:%s, ��������:%s"), (LPCTSTR)SqlState, (LPCTSTR)Msg);
		::MessageBox(NULL, str, _T("���� ����"), 0);
		ii++;
	}
	return Ret;
}

bool Add()
{
	TCHAR szSQL[256] = { 0, };

	SQLTCHAR UserID[11] = _T("������");
	SQLTCHAR UserPW[11] = _T("������1");

	_stprintf((TCHAR*)szSQL, _T("Insert into UserList values('%s', '%s')"), UserID, UserPW);
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)szSQL, SQL_NTS) == SQL_SUCCESS)
	{
		//��ȸ�Ҷ� Ŀ���� ��ġ�κ��� ������ ��ȸ�Ѵ�.
		//�׷��� Ŀ���� �ݾƼ� �ٽ� �˻� �Ҷ� ������ �÷����Ѵ�.
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
	SQLCHAR ID[20] = { 0, };
	SQLCHAR Pass[20] = { 0, };
	SQLLEN lID, lPass;
	//SQL_C_CHAR ª�� �ؽ�Ʈ SQL_C_ULONG ����.
	//1�� �ʵ� 2�� �ʵ�.
	SQLBindCol(g_hStmt, 1, SQL_C_CHAR, ID, sizeof(ID), &lID);
	SQLBindCol(g_hStmt, 2, SQL_C_CHAR, Pass, sizeof(Pass), &lPass);

	//?marker ��ȯ�� = ���ڰ� ?�� ��ȣ�� �ִ�.
	//SQL_PARAM_OUTPUT ����Ÿ��, SQL_PARAM_INPUT �Ű�����
	//1, 2 �� ? ��Ŀ�� ������� �־���Ѵ�.
	SWORD sReturn = -1;
	SQLLEN cbRetParam = SQL_NTS;
	SQLBindParameter(g_hStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0,
		&sReturn, sizeof(sReturn), &cbRetParam);

	SQLCHAR UserID[10] = "���Ҵ�";
	SQLBindParameter(g_hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_LONGVARCHAR, 10, 0,
		UserID, sizeof(UserID), NULL);

	SQLTCHAR sql[] = _T("{?=CALL dbo.usp_pass(?)}");
	SQLRETURN ret2 = SQLPrepare(g_hStmt, sql, SQL_NTS);

	SQLRETURN ret = SQLExecute(g_hStmt);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
	{
		ret = HandleDiagnosticRecord();
		return;
	}

	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		printf("\n[%d] , %s : [%s]", sReturn, ID, Pass);
	}

	while ((ret = SQLMoreResults(g_hStmt)) != SQL_NO_DATA);
	printf("\nRet%d", sReturn);

	//��ȸ�Ҷ� Ŀ���� ��ġ�κ��� ������ ��ȸ�Ѵ�.
	//�׷��� Ŀ���� �ݾƼ� �ٽ� �˻� �Ҷ� ������ �÷����Ѵ�.
	SQLCloseCursor(g_hStmt);
}

bool Delete()
{
	TCHAR szSQL[256] = _T("Delete from USERLIST where USERID = '������'");
	SQLTCHAR Name[11] = _T("������");

	//_stprintf((TCHAR*)szSQL, _T("Delete from tblCigar where name = '%s'"), Name);
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)szSQL, SQL_NTS) == SQL_SUCCESS)
	{
		//��ȸ�Ҷ� Ŀ���� ��ġ�κ��� ������ ��ȸ�Ѵ�.
		//�׷��� Ŀ���� �ݾƼ� �ٽ� �˻� �Ҷ� ������ �÷����Ѵ�.
		SQLCloseCursor(g_hStmt);
		Select();
		return true;
	}
	SQLCloseCursor(g_hStmt);
	return false;
}

bool Update()
{
	TCHAR szSQL[256] = _T("Update USERLIST set USERID = '���Ҵ�' where USERID = '������'");
	SQLTCHAR Name[11] = _T("������");

	//_stprintf((TCHAR*)szSQL, _T("Update tblCigar where name = '%s'"), Name);
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)szSQL, SQL_NTS) == SQL_SUCCESS)
	{
		//��ȸ�Ҷ� Ŀ���� ��ġ�κ��� ������ ��ȸ�Ѵ�.
		//�׷��� Ŀ���� �ݾƼ� �ٽ� �˻� �Ҷ� ������ �÷����Ѵ�.
		SQLCloseCursor(g_hStmt);
		Select();
		return true;
	}
	SQLCloseCursor(g_hStmt);
	return false;
}

void main()
{
	//ȯ�� �ڵ�
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &g_hEnv) != SQL_SUCCESS)
		return;

	if (SQLSetEnvAttr(g_hEnv, SQL_ATTR_ODBC_VERSION,
		(SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
		return;

	//���� �ڵ�
	//ȯ���ڵ��� ���� ����� ȯ���ڵ��� ���ؼ� �����ڵ��� ����
	//ȯ���ڵ��� �θ� NULL�̰� �����ڵ� �θ�� ȯ���ڵ��̴�.

	SQLAllocHandle(SQL_HANDLE_DBC, g_hEnv, &g_hDbc);

	// ���� --> *.mdb,"��ĭ�پ�� �Ѵ�."*.accdb
	SQLTCHAR szInCon[256] = _T("DSN=kimyura");

	SQLTCHAR szOutcon[256] = { 0, };
	SQLSMALLINT cbCon;
	//_stprintf(InCon, _T("%s"), _T("Driver={SQL Server};Server=shader.kr;Address=112.216.123.139,1433;Network=dbmssocn;Database=cigarette;Uid=sa;Pwd=kgca!@34;"));

	SQLRETURN ret = SQLConnect(g_hDbc, (SQLTCHAR*)L"kimyura", SQL_NTS,
		(SQLTCHAR*)L"sa", SQL_NTS, (SQLTCHAR*)L"kgca!@34", SQL_NTS);


	if (ret != SQL_SUCCESS && (ret != SQL_SUCCESS_WITH_INFO))
	{
		return;
	}


	//��� �ڵ�
	SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hStmt);

	while (1)
	{
		int iSelect;
		printf("\n0:���, 1:�߰�, 2:����, 3: ����\n");
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