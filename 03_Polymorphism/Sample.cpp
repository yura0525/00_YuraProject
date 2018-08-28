#include "TFigure.h"
#include "T_Triangle.h"
#include "T_Rectangle.h"
#include "T_Elipse.h"
#include <iostream>
using namespace std;

void main()
{
	TFigure * pFigure[3] = { new T_Triangle(), new T_Rectangle(), new T_Elipse() };

	for (int i = 0; i < 3; i++)
	{
		pFigure[i]->Draw();
	}

	for (int i = 0; i < 3; i++)
	{
		delete pFigure[i];
	}
}
