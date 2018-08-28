#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
//전방선언
class TToken;
class TValue
{
private:
	char*	m_szValue;
	TValue*	m_pNext;
	friend class TToken;

public:
	TValue(char*);
	~TValue();
};

