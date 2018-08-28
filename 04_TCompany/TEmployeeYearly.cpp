#include "TEmployeeYearly.h"

TEmployeeYearly::TEmployeeYearly(int _iGrade, const char* _name, ePosition _ePosition) : TEmployee(_name, _ePosition)
{
	m_iGrade = _iGrade;
}

TEmployeeYearly::~TEmployeeYearly()
{
}

int TEmployeeYearly::GetMoney()
{
	int money = (20000000 + (10000000 * GetPosition()) + (m_iGrade * 5000000))/ 12;
	return money;
}