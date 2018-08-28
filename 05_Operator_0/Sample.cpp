#include "TStudent.h"
#include <stdlib.h>
#include <conio.h>

void main()
{
	TStudent a[3];

	a[0].SetSubject(rand() % 100, rand() % 100, rand() % 100);
	a[1].SetSubject(rand() % 100, rand() % 100, rand() % 100);
	a[2].SetSubject(rand() % 100, rand() % 100, rand() % 100);

	//+연산자를 재정의
	TStudent d;
	for (int i = 0; i < 3; i++)
	{
		d = d + a[i];		//TStudent	operator+(TStudent& data);	
	}
	d.Show();

	TStudent tmp2 = d++;
	tmp2.Show();

	TStudent tmp3 = ++d;
	tmp3.Show();

	d = d + 30;
	d.Show();

	d = 30 + d;		//friend TStudent operator+(int iValue, TStudent& data);
	d.Show();
	_getch();
}