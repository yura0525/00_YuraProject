#include "TToken.h"
#include <string.h>

TToken::TToken()
{

}
TToken::TToken(char* pData)
{
	m_iCount = 0;
	char* temp;
	TValue* pLink;
	temp = strtok(pData, " ");
	if (temp != NULL)
	{
		m_pValue	= new TValue(temp);
		pLink		= m_pValue;
		for (m_iCount = 1; temp = strtok(NULL, " "); m_iCount++)
		{
			pLink->m_pNext = new TValue(temp);
			pLink = pLink->m_pNext;
		}
	}
}


TToken::~TToken()
{
	TValue * pDel;
	TValue * pLink = m_pValue;
	while (pLink != NULL)
	{
		pDel = pLink;
		pLink = pLink->m_pNext;
		delete pDel;
	}
}

void TToken::Show()
{
	for (int iCnt = 0; iCnt < m_iCount; iCnt++)
	{
		std::cout << m_pValue->m_szValue << " ";
		m_pValue = m_pValue->m_pNext;
	}
	std::cout << std::endl;
}

void TToken::Show(TValue *pValue)
{
	TValue* pLink = pValue;
	while (pLink != NULL)
	{
		std::cout << pLink->m_szValue << " ";
		pLink = pLink->m_pNext;
	}
}
