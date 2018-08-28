#include "TSwap.h"
#include "TArray.h"
#include <iostream>
#include <conio.h>
using namespace std;

int main()
{
	char aArray[] = "kgca";
	char bArray[] = "game";

	cout << "Before Swap() aArray = " << aArray << " bArray = " << bArray << endl;
	Swap(aArray, bArray);
	cout << "After Swap() aArray = " << aArray << " bArray = " << bArray << endl;

	int a = 10;
	int b = 20;

	cout << "Before Swap() a = " << a << " b = " << b << endl;
	Swap(&a, &b);
	cout << "After Swap() a = " << a << " b = " << b<<endl;

	TArray<int> data(10);
	int val;
	for(int i = 0; i < 10; i++)
	{
		if (!data.SetData(i, i))
			cout << "Fail to SetData()" << endl;
		if (!data.GetData(i, val))
			cout << "Fail to GetData()" << endl;
		else
			cout << "Data[" << i << "] = " << val << endl;
	}
	getchar();
	return 0;
}
