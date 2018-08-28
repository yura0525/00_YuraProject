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
		return "���";
	case eDaeri:
		return "�븮";
	case eGuajang:
		return "����";
	case eBujang:
		return "����";
	case eEsa:
		return "�̻�";
	default:
		return "���";
	}
	return "";
}
