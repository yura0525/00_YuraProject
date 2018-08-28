#include "TStack.h"
using namespace std;

template<class T>
T TStack<T>::g_NULL = NULL;

template<class T>
TStack<T>::TStack() :m_iTop(-1)
{
}

template<class T>
TStack<T>::~TStack()
{
}

template<class T>
T TStack<T>::Push(T iData)
{
	if ((g_iMaxStack-1) == m_iTop )
	{
		std::cout << "Stack Overflow" << std::endl;
		return g_NULL;
	}
	m_iStackArray[++m_iTop] = iData;
	return iData;
}

template<class T>
T& TStack<T>::Pop()
{
	if (m_iTop < 0)
	{
		std::cout << "Stack Underflow" << std::endl;
		return g_NULL;
	}
	T& iRet = m_iStackArray[m_iTop--];
	return iRet;
}

template<class T>
void TStack<T>::Show()
{
	cout << std::endl;
	for (int i = m_iTop; i >= 0; i--)
	{
		cout << m_iStackArray[i]->m_iID << " " << std::endl;
	}
}