#include "T_Triangle.h"
#include <iostream>
using namespace std;



T_Triangle::T_Triangle()
{
	cout << "T_Triangle::T_Triangle() 생성자함수" << endl;
}


T_Triangle::~T_Triangle()
{
	cout << "T_Triangle::~T_Triangle() 소멸자함수" << endl;
}

void T_Triangle::Draw()
{
	cout << "T_Triangle::Draw()" << endl;
}