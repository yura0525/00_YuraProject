#pragma once
#include <iostream>
using namespace std;

struct YNode
{
	int		m_iValue;
	YNode*	m_pNext;

	YNode(int iValue)
	{
		m_iValue = iValue;
		m_pNext = NULL;
	}
};

class YLinkedList
{
public:
	YNode*	m_pHead = NULL;
	YNode*	m_pTail = NULL;
	
public:
	YLinkedList();
	virtual ~YLinkedList();

	virtual void AddNode(int iValue);
	virtual void AddNode(YNode* pNode);
	virtual YNode* DeleteNode(int iValue);
	virtual YNode* DeleteNode(YNode* pNode);

	virtual void PrintNode(YNode* pHead = NULL);
};

class YCircularList:public YLinkedList
{
public:
	int		m_iNumCounter;
public:
	YCircularList();
	~YCircularList();

	void AddNode(int iValue);
	void AddNode(YNode* pNode);
	YNode* DeleteNode(int iValue);
	YNode* DeleteNode(YNode* pNode);

	void PrintNode(YNode* pHead = NULL);
};