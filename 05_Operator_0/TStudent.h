#pragma once
class TStudent
{
private:
	int m_iKor;	//0
	int m_iEng;	//1
	int m_iMath;//2

	int m_iTotal;//3
public:

	TStudent();
	~TStudent();

	TStudent	operator++(int iData);	//후위연산자++의함수	a++
										//int iData는 의미가 없고 전위이냐 후위 구분하는 용도이다
	TStudent&	operator++();			//전위 연산자++의 함수	++a

	//전역함수를 프렌드 지원 ex) 2 + TStudent a를 지원하기 위한 전역함수
	friend TStudent operator+(int iValue, TStudent& data);

	TStudent&	operator+(int data);		//a + 2
	TStudent	operator+(TStudent& data);	
	TStudent&	operator+=(TStudent& data);	//다시 대입(+=)하기때문에 &를 리턴한다.

	//인덱싱 지원
	int			operator[](int index);

	void SetSubject(int kor, int eng, int mat);

	void SetTotal(int iValue)
	{
		m_iTotal = iValue;
	}
	int GetTotal();
	void Show();
};

//friend TStudent operator+(int iValue, TStudent& data);
TStudent operator+(int iValue, TStudent& data);

