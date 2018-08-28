#include <iostream>
using namespace std;

#define SAFE_NEW(p, N)				{	if(!p){ p = new N;}	}
#define SAFE_DEL(p)					{	if(p){ delete p; (p) = NULL;}	}

#define SAFE_NEW_VALUE(p, N, v)		{	if(!p){ p = new N(v);}	}

#define SAFE_NEW_ARRAY (p, N, C)	{	if(!p){ p = new N[C];}	if(p) {memset(p, 0, (sizeof(p) * C));}	}
#define SAFE_DEL_ARRAY(p)			{	if(p){  delete []p; (p) = NULL;}	}

#define SAFE_ZERO(p)				{	p = NULL;	}

struct TNode
{
	int m_iValue;
	int m_iDepth;
	TNode* m_pLeft;
	TNode* m_pRight;

	TNode(int iData) : m_iValue(iData), m_iDepth(0), m_pLeft(0), m_pRight(0)
	{
	}
};

TNode* g_pRoot = NULL;
int g_iIndex = 0;

bool Add(TNode* pParentNode)
{
	if (pParentNode->m_iDepth > 2)
		return false;

	TNode* pLeft = 0;
	SAFE_NEW_VALUE(pLeft, TNode, ++g_iIndex);
	pLeft->m_iDepth = pParentNode->m_iDepth + 1;
	pParentNode->m_pLeft = pLeft;

	TNode* pRight = 0;
	SAFE_NEW_VALUE(pRight, TNode, ++g_iIndex);
	pRight->m_iDepth = pParentNode->m_iDepth + 1;
	pParentNode->m_pRight = pRight;

	/*TNode* pLeft = new TNode(++g_iIndex);
	pLeft->m_iDepth = pParentNode->m_iDepth + 1;
	pParentNode->m_pLeft = pLeft;

	TNode* pRight = new TNode(++g_iIndex);
	pRight->m_iDepth = pParentNode->m_iDepth + 1;
	pParentNode->m_pRight = pRight;*/

	return true;
}

//프리오더(전위)방식으로 트리를 생성
void Build(TNode* pParentNode)
{
	if (Add(pParentNode))
	{
		Build(pParentNode->m_pLeft);
		Build(pParentNode->m_pRight);
	}
}
//탐색은 3가지 방법으로 탐색
void PRINT(TNode* pParentNode)
{
	//탐색하는 3가지 종류
	//전위 탐색
	/*if (pParentNode != NULL )
	{
	cout << pParentNode->m_iValue << endl;
	PRINT(pParentNode->m_pLeft);
	PRINT(pParentNode->m_pRight);
	}*/

	//중위 탐색
	/*if (pParentNode != NULL)
	{
	PRINT(pParentNode->m_pLeft);
	cout << pParentNode->m_iValue << endl;
	PRINT(pParentNode->m_pRight);
	}*/

	//후위 탐색
	if (pParentNode != NULL)
	{
		PRINT(pParentNode->m_pLeft);
		PRINT(pParentNode->m_pRight);
		cout << pParentNode->m_iValue << endl;
	}
}
//삭제는 후위방식으로 삭제한다.
void DEL_ALL(TNode* pParentNode)
{
	if (pParentNode == NULL)	return;

	DEL_ALL(pParentNode->m_pLeft);
	DEL_ALL(pParentNode->m_pRight);
	SAFE_DEL(pParentNode);
}

int g_iData[10] = { 5,0,4,7,8,3,9,2,6,1 };

void BinaryNode(TNode* pParentNode, int iData)
{
	if (pParentNode == 0) return;
	//pParentNode->m_iValue <= iData	-->오른쪽 노드 추가
	//pParentNode->m_iValue > iData		-->왼쪽 노드 추가

	if (pParentNode->m_iValue > iData)
	{
		if (pParentNode->m_pLeft == 0)
		{
			pParentNode->m_pLeft = new TNode(iData);
		}
		else
		{
			BinaryNode(pParentNode->m_pLeft, iData);
		}
	}
	else
	{
		if (pParentNode->m_pRight == 0)
		{
			pParentNode->m_pRight = new TNode(iData);
		}
		else
		{
			BinaryNode(pParentNode->m_pRight, iData);
		}
	}
}
int main()
{
	SAFE_NEW_VALUE(g_pRoot, TNode, g_iData[g_iIndex++]);
	for (int i = 1; i < 10; i++)
	{
		BinaryNode(g_pRoot, g_iData[i]);
	}

	/*SAFE_NEW_VALUE(g_pRoot, TNode, 0);
	Build(g_pRoot);
	DEL_ALL(g_pRoot);
	SAFE_ZERO(g_pRoot);*/
	PRINT(g_pRoot);
	return 0;
}