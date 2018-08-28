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

//Adapter����
//1. ���߻��(public TextView)���� �ص��ǰ�,
//2.TextView m_tv;����� �������־ �������� �ص� �ȴ�.
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