#include "YLinkedList.h"

YLinkedList::YLinkedList()
{
	m_pHead = new YNode(0);
	m_pTail = m_pHead;
}

YLinkedList::~YLinkedList()
{
	YNode * pDelNode;

	while (m_pHead != NULL && m_pHead->m_pNext != NULL )
	{
		pDelNode = m_pHead;
		m_pHead = m_pHead->m_pNext;
		delete pDelNode;
	}

	if (m_pHead != NULL)
		delete m_pHead;
	m_pHead = m_pTail = NULL;
}

void YLinkedList::AddNode(int iValue)
{
	YNode * pNode = new YNode(iValue);
	m_pTail->m_pNext = pNode;
	m_pTail = m_pTail->m_pNext;
	
}

void YLinkedList::AddNode(YNode* pNode)
{
	m_pTail->m_pNext = pNode;
	m_pTail = m_pTail->m_pNext;
}

void YLinkedList::PrintNode(YNode* pHead)
{
	if (m_pHead == NULL)
		return;

	if (pHead == NULL)
		pHead = m_pHead;

	for (YNode* pNode = pHead; pNode!= NULL; pNode = pNode->m_pNext)
	{
		cout << pNode->m_iValue << endl;
	}
}

YNode* YLinkedList::DeleteNode(int iValue)
{
	if (m_pHead == NULL)
		return NULL;

	if (m_pHead->m_iValue == iValue)
	{
		YNode* pDelNode = m_pHead;
		m_pHead = m_pHead->m_pNext;
		delete pDelNode;
		return m_pHead;
	}

	YNode* pPrevNode = m_pHead;
	while (pPrevNode != NULL 
		&& pPrevNode->m_pNext != NULL)
	{
		if (pPrevNode->m_pNext->m_iValue == iValue)
		{
			break;
		}
		pPrevNode = pPrevNode->m_pNext;
	}

	//지우는 데이터가 없을 경우
	if (pPrevNode->m_pNext->m_iValue != iValue)
		return NULL;

	return DeleteNode(pPrevNode);
}

YNode* YLinkedList::DeleteNode(YNode* pPrevNode)
{
	if (pPrevNode == NULL || pPrevNode->m_pNext == NULL)
	{
		return NULL;
	}

	YNode * pDelNode = pPrevNode->m_pNext;

	if (pDelNode == m_pHead)
	{
		m_pHead = m_pHead->m_pNext;
		delete pDelNode;
		return m_pHead;
	}

	if (pDelNode == m_pTail)
	{
		m_pTail = pPrevNode;
		m_pTail->m_pNext = NULL;
		delete pDelNode;
		return m_pTail;
	}

	if (pPrevNode->m_pNext != NULL && pDelNode->m_pNext != NULL)
		pPrevNode->m_pNext = pDelNode->m_pNext;
	
	YNode * pNext = pDelNode->m_pNext;
	delete pDelNode;

	return pNext;
}

YCircularList::YCircularList()
{
	m_iNumCounter = 1;
}
YCircularList::~YCircularList()
{
	YNode * pDelNode;

	while (m_pHead != NULL && m_pHead->m_pNext != NULL)
	{
		pDelNode = m_pHead;
		if (m_pHead->m_pNext != NULL)
			m_pHead = m_pHead->m_pNext;
		delete pDelNode;
	}

	if (m_pHead != NULL)
		delete m_pHead;
	m_pHead = m_pTail = NULL;
	m_iNumCounter = 0;
}
void YCircularList::AddNode(int iValue)
{
	YLinkedList::AddNode(iValue);
	m_iNumCounter++;
	m_pTail->m_pNext = m_pHead;
}
void YCircularList::AddNode(YNode* pNode)
{
	YLinkedList::AddNode(pNode);
	m_pTail->m_pNext = m_pHead;
	m_iNumCounter++;
}
YNode* YCircularList::DeleteNode(int iValue)
{
	YNode* pNextNode =  YLinkedList::DeleteNode(iValue);
	if (pNextNode != NULL && pNextNode == m_pTail)
	{
		pNextNode->m_pNext = m_pHead;
	}
	--m_iNumCounter;
	return pNextNode;
}
YNode* YCircularList::DeleteNode(YNode* pNode)
{
	YNode* pNextNode = YLinkedList::DeleteNode(pNode);
	if (pNextNode != NULL && pNextNode == m_pTail)
	{
		pNextNode->m_pNext = m_pHead;
	}
	--m_iNumCounter;
	return pNextNode;
}

void YCircularList::PrintNode(YNode* pHead)
{
	if (m_pHead == NULL)
		return;

	if (pHead == NULL)
		pHead = m_pHead;

	int iCnt = 0;
	for (YNode* pNode = pHead; pNode != NULL; pNode = pNode->m_pNext)
	{
		if (iCnt++ >= m_iNumCounter)
			break;
		cout << "PrintNode: " <<pNode->m_iValue << endl;
	}
}