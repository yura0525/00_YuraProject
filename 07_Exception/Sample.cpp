#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
class TException
{
public:
	int m_iError;
	char m_czError[256];
public:
	TException(int error) : m_iError(error)
	{
		
	}
	void Show()
	{
		switch (m_iError)
		{
		case 0:
			strcpy(m_czError, "0���� ����");
			break;
		case 1:
			strcpy(m_czError, "��ȿ������ �ʰ���");
			break;
		}
		cout << m_czError << endl;
	}
};

void DivideA(int a, int b, int& c)
{
	if (b == 0)
		throw TException(0);

	if (b != 0)
	{
		c = a / b;
		if (c < 0.0f)	TException(1);
	}
	throw TException(1);
}
void main()
{
	int a = 10;
	int b = 0;
	int c = 0;

	//try{ throw } catch(){}
	//���δٸ������͸� throw�Ҽ� �ִ�.
	try
	{
		DivideA(a, b, c);
		cout << c;
	}
	/*catch (_exception e)
	{
	}*/
	catch (TException e)
	{
		e.Show();
	}
	/*catch (bool bRet)
	{
		cout << "bool ������!";
	}
	catch (int bRet)
	{
		cout << "int ������!";
	}
	catch (char bRet)
	{
		cout << "char ������!";
	}*/
}