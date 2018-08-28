#include "Point.h"
#include <iostream>
using namespace std;

Point::Point()
{
	x = 0;
	y = 0;
}
Point::~Point()
{
	cout << "Point::~Point()" << endl;
}
void Point::SetPosition(int a, int b)
{
	x = a;
	y = b;
}

void Point::Move(int a, int b)
{
	x += a;
	y += b;
}

void Point::Show()
{
	cout << x << " " << y << endl;
}