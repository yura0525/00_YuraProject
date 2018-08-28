#include "TValue.h"
#include <string.h>

TValue::TValue(char* pData)
{
	int iLength = strlen(pData) + 1;
	m_szValue = new char[iLength];
	strcpy(m_szValue, pData);
	m_pNext = NULL;
}


TValue::~TValue()
{
	delete[] m_szValue;
}
