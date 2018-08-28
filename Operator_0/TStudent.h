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

	TStudent	operator++(int iData);	//����������++���Լ�	a++
										//int iData�� �ǹ̰� ���� �����̳� ���� �����ϴ� �뵵�̴�
	TStudent&	operator++();			//���� ������++�� �Լ�	++a

	//�����Լ��� ������ ���� ex) 2 + TStudent a�� �����ϱ� ���� �����Լ�
	friend TStudent operator+(int iValue, TStudent& data);

	TStudent&	operator+(int data);		//a + 2
	TStudent	operator+(TStudent& data);	
	TStudent&	operator+=(TStudent& data);	//�ٽ� ����(+=)�ϱ⶧���� &�� �����Ѵ�.

	//�ε��� ����
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

