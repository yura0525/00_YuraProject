#include "T_Rectangle.h"
#include <iostream>
using namespace std;

T_Rectangle::T_Rectangle()
{
	cout << "T_Rectangle::T_Rectangle() 생성자함수" << endl;
}


T_Rectangle::~T_Rectangle()
{
	cout << "T_Rectangle::~T_Rectangle() 소멸자함수" << endl;
}

void T_Rectangle::Draw()
{
	cout << "T_Rectangle::Draw()" << endl;
}
