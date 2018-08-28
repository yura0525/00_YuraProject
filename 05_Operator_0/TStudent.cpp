#include "TStudent.h"
#include <iostream>
using namespace std;

//friend TStudent operator+(int iValue, TStudent& data);함수이다.
TStudent operator+(int iValue, TStudent& data)
{
	TStudent ret;
	ret.SetSubject(data[0], data[1], data[2]);
	ret.SetTotal(data[3] + iValue);

	return ret;
}

TStudent::TStudent()
{
	m_iKor = 0;
	m_iEng = 0;
	m_iMath = 0;
}


TStudent::~TStudent()
{
}

TStudent TStudent::operator+(TStudent& data)
{
	TStudent temp;
	temp.SetSubject((m_iKor + data.m_iKor), (m_iEng + data.m_iEng), (m_iMath + data.m_iMath));

	return temp;
}

TStudent& TStudent::operator+=(TStudent& data)
{
	m_iKor += data.m_iKor;
	m_iEng += data.m_iEng;
	m_iMath += data.m_iMath;

	m_iTotal = m_iKor + m_iEng + m_iMath;
	return *this;
}
void TStudent::SetSubject(int kor, int eng, int mat)
{
	m_iKor = kor;
	m_iEng = eng;
	m_iMath = mat;

	m_iTotal = m_iKor + m_iEng + m_iMath;
}
int TStudent::GetTotal()
{
	return m_iTotal;
}

void TStudent::Show()
{
	cout << "국어 = " << m_iKor << "영어 = " << m_iEng 
		<< "수학 = " << m_iMath << "총점 = " << m_iTotal << endl;
}
int TStudent::operator[](int index)
{
	if (index > 3 || index < 0) return -1;

	if (index == 0) return m_iKor;
	if (index == 1) return m_iEng;
	if (index == 2) return m_iMath;
	if (index == 3) return m_iTotal;

	return -1;
}
//후위연산자++의함수	a++
//int iData는 의미가 없고 전위이냐 후위 구분하는 용도이다
TStudent TStudent::operator++(int iData)
{
	TStudent ret = *this;
	m_iTotal += 10;
	return ret;
}
//전위 연산자++의 함수	++a
TStudent& TStudent::operator++()
{
	m_iTotal += 10;
	return *this;
}

TStudent& TStudent::operator+(int data)
{
	m_iTotal += data;
	return *this;
}

