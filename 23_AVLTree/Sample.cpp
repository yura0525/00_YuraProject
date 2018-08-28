#include <iostream>
#include "BTree.h"

using namespace std;


int main()
{
	TNode* pRoot = 0;

	for (int i = 0; i < 10; i++)
	{
		pRoot = Insert(pRoot, i);
	}

	return 0;
}