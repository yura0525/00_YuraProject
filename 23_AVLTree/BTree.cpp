#include "BTree.h"



BTree::BTree()
{
}


BTree::~BTree()
{
}


TNode* ReBalance(TNode* nNode)
{
	//ReBalance(nNode);
	return 0;
}

int getHeight(TNode* pNode)
{
	if (pNode == NULL)
		return 0;

	return pNode->m_iDepth;
}

TNode* updateHeight(TNode* pNode)
{
	pNode->m_iDepth = 1 + max(getHeight(pNode->m_pLeft), getHeight(pNode->m_pRight));
	return pNode;
}

TNode* LRotate(TNode* pRoot)
{
	TNode* x = pRoot->m_pRight;
	TNode* t = x->m_pLeft;

	pRoot->m_pRight = t;
	x->m_pLeft = pRoot;

	x = updateHeight(x);
	pRoot = updateHeight(pRoot);

	return x;
}
TNode* RRotate(TNode* pRoot)
{
	TNode* x = pRoot->m_pLeft;
	TNode* t = x->m_pRight;

	pRoot->m_pLeft = t;
	x->m_pRight = pRoot;

	x = updateHeight(x);
	pRoot = updateHeight(pRoot);

	return 0;
}

TNode* Insert(TNode* pRoot, int iData)
{
	if (pRoot == NULL)
		return new TNode(iData);

	if (pRoot->m_iData < iData)
	{
		pRoot->m_pRight = Insert(pRoot->m_pRight, iData);
	}
	else if (pRoot->m_iData == iData)
	{
		return pRoot;
	}
	else
	{
		pRoot->m_pLeft = Insert(pRoot->m_pLeft, iData);
	}

	pRoot = updateHeight(pRoot);

	int iBalanceFactor = getHeight(pRoot->m_pLeft) - getHeight(pRoot->m_pRight);
	//std::cout << "getHeight(pRoot->m_pLeft) = " << getHeight(pRoot->m_pLeft) << " getHeight(pRoot->m_pRight) " << getHeight(pRoot->m_pRight) 
	//	<<"iBalanceFactor = " << iBalanceFactor << std::endl;

	if (iBalanceFactor > 1 &&
		(pRoot->m_pLeft->m_iData) < iData)
	{
		pRoot->m_pLeft = LRotate(pRoot->m_pLeft);
		return RRotate(pRoot);
	}
	else if (iBalanceFactor > 1 &&
		(pRoot->m_pLeft->m_iData) > iData)
	{
		return RRotate(pRoot);
	}
	else if (iBalanceFactor < -1 &&
		(pRoot->m_pRight->m_iData) < iData)
	{
		return LRotate(pRoot);
	}
	else  if (iBalanceFactor < -1 && 
		(pRoot->m_pRight->m_iData) > iData)
	{
		pRoot->m_pRight = RRotate(pRoot->m_pRight);
		return LRotate(pRoot);
	}

	return pRoot;
}
