#define _CRT_SECURE_NO_WARNINGS
#include "TEmployee.h"
#include <string>

TEmployee::TEmployee(const char* _name, ePosition _ePosition)
{
	size_t nSize = strlen(_name) + 1;
	m_Name = new char[nSize];
	strcpy(m_Name, _name);

	m_ePosition = _ePosition;
}


TEmployee::~TEmployee()
{
	delete []m_Name;
}

const char* TEmployee::GetPositionString()
{
	switch (m_ePosition)
	{
	case eSawon:
		return "사원";
	case eDaeri:
		return "대리";
	case eGuajang:
		return "과장";
	case eBujang:
		return "부장";
	case eEsa:
		return "이사";
	default:
		return "사원";
	}
	return "";
}
