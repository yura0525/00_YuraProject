#include "TEmployeeMonthly.h"

TEmployeeMonthly::TEmployeeMonthly(int _iYear, const char* _Name, ePosition _Position) : TEmployee(_Name, _Position)
{
	m_iYear = _iYear;
}


TEmployeeMonthly::~TEmployeeMonthly()
{
}

int TEmployeeMonthly::GetMoney()
{
	int money = 2000000 + (GetPosition() * 20) + (m_iYear * 5);
	return money;
}

