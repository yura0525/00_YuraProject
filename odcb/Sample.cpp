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
bool Add();
bool Delete();
bool Update();
//TCHAR szSQL[] = _T("Insert into tblCigar(name, price, korean) values('bbb', 3000, 1)");

//select..from table
//select..from table where ?=?;
void Select()
{
	SQLTCHAR Name[20] = { 0, };
	int iPrice, isKorean;
	SQLLEN lName, lPrice, lKorean;
	//SQL_C_CHAR ª�� �ؽ�Ʈ SQL_C_ULONG ����.
	//1�� �ʵ� 2�� �ʵ�.
	SQLBindCol(g_hStmt, 1, SQL_C_CHAR, Name, sizeof(Name), &lName);
	SQLBindCol(g_hStmt, 2, SQL_C_ULONG, &iPrice, sizeof(iPrice), &lPrice);
	SQLBindCol(g_hStmt, 3, SQL_C_ULONG, &isKorean, sizeof(isKorean), &lKorean);

	//tblCigar���̺�κ��� name(�ʵ��)�� �����ض�.
	//select...from...where... '���ڿ�' 
	//if (SQLExecDirect(g_hStmt, (SQLTCHAR*)_T("select name,price,korean from tblCigar where name='This'"), SQL_NTS) != SQL_SUCCESS)
	//{
	//	return;
	//}
	if (SQLExecDirect(g_hStmt, (SQLTCHAR*)_T("select * from tblCigar"), SQL_NTS) != SQL_SUCCESS)
	{
		return;
	}

	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		printf("\n%s : price[%d][%d]", Name, iPrice, isKorean);
	}

	//��ȸ�Ҷ� Ŀ���� ��ġ�κ��� ������ ��ȸ�Ѵ�.
	//�׷��� Ŀ���� �ݾƼ� �ٽ� �˻� �Ҷ� ������ �÷����Ѵ�.
	SQLCloseCursor(g_hStmt);
}

bool Add()
{
	TCHAR szSQL[256] = { 0, };
	SQLTCHAR Name[21] = _T("�ݿ�");
	SQLINTEGER Price = 3000;
	BOOL Korean = FALSE;

	_stprintf((TCHAR*)szSQL, _T("Insert into tblCigar(name, price, korean) values('%s', %d, %d)"), Name, Price, Korean);
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

bool Delete()
{
	TCHAR szSQL[256] = _T("Delete from tblCigar where name = '�ݿ�'");
	SQLTCHAR Name[21] = _T("�ݿ�");

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
	TCHAR szSQL[256] = _T("Update tblCigar set name = '�ݿ���' where name = '�ݿ�'");
	SQLTCHAR Name[21] = _T("�ݿ�");
	SQLINTEGER Price = 3000;
	BOOL Korean = FALSE;

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
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &g_hEnv) != SQL_SUCCESS )
		return;
	
	if (SQLSetEnvAttr(g_hEnv, SQL_ATTR_ODBC_VERSION,
		(SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
		return;

	//���� �ڵ�
	//ȯ���ڵ��� ���� ����� ȯ���ڵ��� ���ؼ� �����ڵ��� ����
	//ȯ���ڵ��� �θ� NULL�̰� �����ڵ� �θ�� ȯ���ڵ��̴�.
	
	SQLAllocHandle(SQL_HANDLE_DBC, g_hEnv, &g_hDbc);
	
	// ���� --> *.mdb,"��ĭ�پ�� �Ѵ�."*.accdb
	SQLTCHAR szInCon[256] = _T("FileDsn=..\\..\\data\\cigarette.mdb.dsn");
	/*_stprintf((TCHAR*)szInCon, 
		_T("DRIVER={Microsoft Access Driver (*.mdb)};DBQ=%s\\cigarette.mdb;"), L"..\\..\\data");*/

	SQLTCHAR szOutcon[256] = { 0, };
	SQLSMALLINT cbCon;
	SQLRETURN ret = SQLDriverConnect(g_hDbc, NULL,
		szInCon, _countof(szInCon), szOutcon, _countof(szOutcon), 
		&cbCon, SQL_DRIVER_NOPROMPT);

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
		case 0: Select();	break;
		case 1:	Add();		break;
		case 2:	Delete();	break;
		case 3:	Update();	 break;
		}
	}

	SQLDisconnect(g_hDbc);

	SQLFreeHandle(SQL_HANDLE_STMT, g_hStmt);
	SQLFreeHandle(SQL_HANDLE_DBC, g_hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, g_hEnv);
}