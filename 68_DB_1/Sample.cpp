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
	SQLTCHAR UserID[11] = _T("������");
	SQLTCHAR UserPW[11] = _T("�����������");

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
	SQLCHAR Name[10] = { 0, };
	SQLCHAR Pass[10] = { 0, };
	SQLLEN lName, lPass;
	//SQL_C_CHAR ª�� �ؽ�Ʈ SQL_C_ULONG ����.
	//1�� �ʵ� 2�� �ʵ�.
	SQLBindCol(g_hStmt, 1, SQL_C_CHAR, Name, sizeof(Name), &lName);
	SQLBindCol(g_hStmt, 2, SQL_C_CHAR, Pass, sizeof(Pass), &lPass);

	//USERLIST���̺�κ��� name(�ʵ��)�� �����ض�.
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)_T("select USERID, USERPW from USERLIST"), SQL_NTS) != SQL_SUCCESS)
	{
		return;
	}

	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		printf("\n%s : [%s]", Name, Pass);
	}

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
	SQLRETURN ret = SQLConnect(g_hDbc, (SQLTCHAR*)L"kimyura", SQL_NTS,
		(SQLTCHAR*)L"sa", SQL_NTS, (SQLTCHAR*)L"kgca!@34", SQL_NTS);


	if (ret != SQL_SUCCESS && (ret != SQL_SUCCESS_WITH_INFO))
		return;

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