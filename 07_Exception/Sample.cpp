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
			strcpy(m_czError, "0으로 나눔");
			break;
		case 1:
			strcpy(m_czError, "유효범위를 초과함");
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
	//서로다른데이터를 throw할수 있다.
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
		cout << "bool 계산오류!";
	}
	catch (int bRet)
	{
		cout << "int 계산오류!";
	}
	catch (char bRet)
	{
		cout << "char 계산오류!";
	}*/
}