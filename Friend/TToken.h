#pragma once
#include "TValue.h"

class TToken
{
public:
	TValue* m_pValue;
	int		m_iCount;
public:
	TToken();
	TToken(char* pData);
	~TToken();
public:
	void Show();
	void Show(TValue *pValue);
};

