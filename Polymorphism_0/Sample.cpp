#include <iostream>
#include "TFigure.h"
#include "TTriangle.h"
#include "TRectangle.h"

class TSpere
{
public:
	int m_Value;
};
class TSpereChild :public TSpere
{
};
int main()
{
	//dynamic_cast
	TFigure *figuare[2];
	figuare[0] = new TTriangle();
	figuare[1] = new TRectangle();
	TSpere *pSpere = new TSpereChild;

	//dynamic_cast --> virtual 1개 사용되어야한다.
	//런타임검사가 가능하다.
	TTriangle *pTriA = (TTriangle*)figuare[0];
	TTriangle *pTriB = dynamic_cast<TTriangle*>(figuare[0]);

	//figuare[0]은 TTriangle로 생성되어 
	//아무 상관 없는 C스타일의 캐스팅으로 (TSpere*)로 형변환시 널이 되어야 하지만,
	//널이되지않고 그냥 캐스팅을 한다.
	TSpere *pTriC = (TSpere*)figuare[0];
	if (pTriC != NULL)
	{
	}
	else
	{
		std::cout << "pTriC == NULL" << std::endl;
	}

	//figuare[0]은 TTriangle로 생성되어 
	//아무 상관 없는 dynamic_cast<TSpere*>로 형변환시 널이된다.
	//그러므로 더 안전한 캐스팅이다.
	TSpere *pTriD = dynamic_cast<TSpere*>(figuare[0]);
	if (pTriD != NULL)
	{
	}
	else
	{
		std::cout << "pTriD == NULL" << std::endl;	//출력
	}
	return 0;
}