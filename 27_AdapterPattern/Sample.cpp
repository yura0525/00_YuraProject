#include "TextView.h"
using namespace std;
class Shape
{
public:
	virtual void Draw(int x, int y, const char *pName) = 0;
};

class LineShape : public Shape
{
public:
	void Draw(int x, int y, const char *pName)
	{
		cout << pName <<"x = " << x << " y = " << y << endl;
	}
public:
	LineShape() {}
	~LineShape() {}
};

//Adapter패턴
//1. 다중상속(public TextView)으로 해도되고,
//2.TextView m_tv;멤버로 가지고있어서 포함으로 해도 된다.
class TTextView : public Shape, public TextView
{
public:
	//TextView m_tv;
	void Draw(int x, int y, const char *pName)
	{
		PRINT(pName);

		cout << "x = " << x << " y = " << y << endl;
	}
public:
	TTextView() {}
	~TTextView() {}
};

int main()
{
	TextView tv;
	tv.PRINT("aaaa");

	Shape* shapeA = new LineShape;
	Shape* shapeB = new TTextView;
	
	shapeA->Draw(10,20, "kgca");
	shapeB->Draw(10, 20, "kgca");

	return 0;
}