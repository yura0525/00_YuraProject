#include "T_Rectangle.h"
#include <iostream>
using namespace std;

T_Rectangle::T_Rectangle()
{
	cout << "T_Rectangle::T_Rectangle() �������Լ�" << endl;
}


T_Rectangle::~T_Rectangle()
{
	cout << "T_Rectangle::~T_Rectangle() �Ҹ����Լ�" << endl;
}

void T_Rectangle::Draw()
{
	cout << "T_Rectangle::Draw()" << endl;
}
