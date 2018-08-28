#pragma once
#include <iostream>
#define max(a,b) ( ((a) > (b)) ? (a) : (b) )

struct TNode
{
	int m_iData;
	int m_iDepth;

	TNode* m_pLeft;
	TNode* m_pRight;

	TNode()
	{
		m_pLeft = m_pRight = NULL;
		m_iData = 0;
		m_iDepth = 0;
	}
	TNode(int iValue)
	{
		m_pLeft = m_pRight = NULL;
		m_iData = iValue;
		m_iDepth = 1;
	}
};

TNode* Insert(TNode* pRoot, int iData);

class BTree
{
public:
	BTree();
	~BTree();
};

