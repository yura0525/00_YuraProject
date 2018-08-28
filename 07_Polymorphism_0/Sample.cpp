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

	//dynamic_cast --> virtual 1�� ���Ǿ���Ѵ�.
	//��Ÿ�Ӱ˻簡 �����ϴ�.
	TTriangle *pTriA = (TTriangle*)figuare[0];
	TTriangle *pTriB = dynamic_cast<TTriangle*>(figuare[0]);

	//figuare[0]�� TTriangle�� �����Ǿ� 
	//�ƹ� ��� ���� C��Ÿ���� ĳ�������� (TSpere*)�� ����ȯ�� ���� �Ǿ�� ������,
	//���̵����ʰ� �׳� ĳ������ �Ѵ�.
	TSpere *pTriC = (TSpere*)figuare[0];
	if (pTriC != NULL)
	{
	}
	else
	{
		std::cout << "pTriC == NULL" << std::endl;
	}

	//figuare[0]�� TTriangle�� �����Ǿ� 
	//�ƹ� ��� ���� dynamic_cast<TSpere*>�� ����ȯ�� ���̵ȴ�.
	//�׷��Ƿ� �� ������ ĳ�����̴�.
	TSpere *pTriD = dynamic_cast<TSpere*>(figuare[0]);
	if (pTriD != NULL)
	{
	}
	else
	{
		std::cout << "pTriD == NULL" << std::endl;	//���
	}
	return 0;
}