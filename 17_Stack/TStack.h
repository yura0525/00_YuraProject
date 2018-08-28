#pragma once
#include <iostream>
static const int g_iMaxStack = 3;

template<class T>
class TStack
{
public:
	T m_iStackArray[g_iMaxStack];
	int m_iTop;

	static T g_NULL;
public:
	T Push(T iData);
	T& Pop();
	void Show();

public:
	TStack();
	virtual ~TStack();
};

