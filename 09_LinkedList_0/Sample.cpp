#include "YLinkedList.h"
#include <iostream>
#include <conio.h>
using namespace std;

int main()
{
	YCircularList list;
	for (int i = 1; i < 5; i++)
	{
		list.AddNode(new YNode(i));
	}

	for (int i = 5; i < 11; i++)
	{
		list.AddNode(i);
	}
	
	list.PrintNode();

	list.DeleteNode(10);
	list.DeleteNode(0);
	list.DeleteNode(5);

	list.PrintNode();

	/*int iStep = 3;
	YNode* pNode = list.m_pHead;
	while (pNode != NULL && pNode->m_pNext != pNode)
	{
		for (int i = 1; i <= iStep-2; i++)
		{
			pNode = pNode->m_pNext;
		}
		if (pNode == NULL)
			break;
		pNode = list.DeleteNode(pNode);
	}
	cout << "######################" << endl;
	list.PrintNode(pNode);
	getchar();*/
	return 0;
}