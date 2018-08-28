#include <iostream>
#include <time.h>

using namespace std;

void DataPrint(int list[], int iNum)
{
	//return;

	for (int i = 0; i < iNum; i++)
	{
		cout << list[i] << " ";
	}
	cout << endl;
}
void SetData(int list[], int iNum)
{
	srand(time(NULL));
	for (int i = 0; i < iNum; i++)
	{
		list[i] = rand() % 100;
	}
}
//�������, ������, ���Լ���
bool Swap(int& a, int& b)
{
	if (a > b)
	{
		int temp = a;
		a = b;
		b = temp;
		return true;
	}
	return false;
}

void Change(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}
	
void SelectionSort(int list[], int iNum)
{
	for (int i = 0; i < iNum; i++)
	{
		for (int j = i+1; j < iNum; j++)
		{
			Swap(list[i], list[j]);
		}
	}
}
void BubbleSort(int list[], int iNum)
{
	for (int i = 0; i < iNum-1; i++)
	{
		bool bflag = false;
		for (int j = 0; j < (iNum-i-1); j++)
		{
			if (Swap(list[j], list[j + 1]))
			{
				bflag = true;
			}
		}
		if (bflag == false)	break;
	}
}

void InsertionSort(int list[], int iNum, int s = 1, int step = 1)
{
	for (int i = s; i < iNum; i+= step)
	{
		int  temp = list[i];
		int j = i - step;
		for (; j >= 0; j-= step)
		{
			if (list[j] > temp)
			{
				list[j + step] = list[j];
				continue;
			}
			break;
		}
		list[j + step] = temp;
	}
}
void ShellSort(int list[], int iNum)
{
	//g = 3 * ((n - 1) / 3) + 1;
	//�׷��� ������ ã�� ����.(g�� �׷��� �����̴�)
	//�׷��� �ʹ� ���Ƶ� ������ �׷��� �ʹ� ��� ������.
	//������ �׷� ������ ã�ƾ��Ѵ�.
	//ShellSort�� �����ؼ� ���������ϰ� �ٽ� �����̴�.
	int g;
	for (g = 1; g < iNum; g = 3 * g + 1);
	g /= 3;

	//cout << "#######ShellSort g = " << g << endl;
	while (g > 0)
	{
		g /= 3;
		for (int s = 0; s < g; s+=g)
		{
			InsertionSort(list, iNum, s, g);	//s�� ����, k�� ����
		}
	}
}
void QuickSort(int list[], int iStart, int iEnd)
{
	int iPivot = list[iStart];
	int iBeginStart = iStart;
	int iBeginEnd = iEnd;
	while (iStart < iEnd)
	{
		while (iPivot <= list[iEnd] && (iStart < iEnd))
		{
			iEnd--;
		}
		while (iPivot >= list[iStart] && (iStart < iEnd))
		{
			iStart++;
		}
		if (iStart > iEnd) break;
		Swap(list[iStart], list[iEnd]);
	}
	Swap(list[iBeginStart], list[iStart]);

	if( iBeginStart < iStart )
		QuickSort(list, iBeginStart, iStart-1);
	if(iBeginEnd > iEnd )
		QuickSort(list, iStart+1, iBeginEnd);
}

void BuildHeap(int list[], int n)
{
	//���� �ε��� K�϶�
	//�θ��� �ε���  = (K-1)/2
	//���� �ڽĳ�� �ε��� = 2K + 1
	//������ �ڽĳ�� �ε��� = 2(K + 1)
	//�̷��� �ؼ� �迭�� �����Ѵ�.
	for (int i = 1; i < n; i++)
	{
		int iChild = i;
		do
		{
			int iRoot = (iChild - 1) / 2;
			if (list[iRoot] < list[iChild])
			{
				Change(list[iRoot], list[iChild]);
			}
			iChild = iRoot;

		} while (iChild != 0);
	}
}

void RebuildHeap(int list[], int n)
{
	//���� �ε��� K�϶�
	//�θ��� �ε���  = (K-1)/2
	//���� �ڽĳ�� �ε��� = 2K + 1
	//������ �ڽĳ�� �ε��� = 2(K + 1)
	//�̷��� �ؼ� �迭�� �����Ѵ�.
	for (int iLast = n-1; iLast >= 0; iLast--)
	{
		//����ū���� ���� �ڷ� ����. �ֻ��� �θ��带 ���� �����ѰŶ� ����.
		Change(list[0], list[iLast]); 

		//�ٽ� ���� �����Ѵ�.
		int iRoot = 0;
		int iChild;
		do
		{
			iChild = 2 * iRoot + 1;
			if ((iChild < (iLast-1)) && list[iChild] < list[iChild+1])
			{
				iChild++;
			}
			if ((iChild < iLast) && list[iRoot] < list[iChild])
			{
				Change(list[iRoot], list[iChild]);
			}
			iRoot = iChild;

		} while (iChild < iLast);
	}
}

void HeapSort(int list[], int n)
{
	BuildHeap(list, n);
	RebuildHeap(list, n);
}

const int g_iMaxCnt = 10;
int main()
{
	int item[g_iMaxCnt];	//_countof
	
	clock_t sTime, eTime;
	
	//SelectionSort
	//SetData(item, _countof(item));
	//sTime = clock();		//1000 : 1��
	//SelectionSort(item, g_iMaxCnt);
	//eTime = clock()- sTime;
	//std::cout << "SelectionSort : " << eTime / (double)CLK_TCK<<endl;
	//////DataPrint(item, g_iMaxCnt);


	//////BubbleSort
	//SetData(item, _countof(item));
	//sTime = clock();		//1000 : 1��
	//BubbleSort(item, g_iMaxCnt);
	//eTime = clock() - sTime;
	//std::cout << "BubbleSort : " << eTime / (double)CLK_TCK << endl;
	//////DataPrint(item, g_iMaxCnt);

	//////InsertionSort
	//SetData(item, _countof(item));
	////DataPrint(item, g_iMaxCnt);
	//sTime = clock();		//1000 : 1��
	//InsertionSort(item, g_iMaxCnt);
	//eTime = clock() - sTime;
	//std::cout << "InsertionSort : " << eTime / (double)CLK_TCK << endl;
	//////DataPrint(item, g_iMaxCnt);


	//////ShellSort
	//SetData(item, _countof(item));
	////DataPrint(item, g_iMaxCnt);
	//sTime = clock();		//1000 : 1��
	//ShellSort(item, g_iMaxCnt);
	//eTime = clock() - sTime;
	//std::cout << "ShellSort : " << eTime / (double)CLK_TCK << endl;
	//////DataPrint(item, g_iMaxCnt);

	////QuickSort
	//SetData(item, _countof(item));
	////DataPrint(item, g_iMaxCnt);
	//sTime = clock();		//1000 : 1��
	//QuickSort(item, 0, (_countof(item)-1));
	//eTime = clock() - sTime;
	//std::cout << "QuickSort : " << eTime / (double)CLK_TCK << endl;
	//DataPrint(item, g_iMaxCnt);

	SetData(item, _countof(item));
	DataPrint(item, g_iMaxCnt);
	sTime = clock();		//1000 : 1��
	HeapSort(item, _countof(item));
	eTime = clock() - sTime;
	std::cout << "HeapSort : " << eTime / (double)CLK_TCK << endl;
	DataPrint(item, g_iMaxCnt);

	return 0;
}