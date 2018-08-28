#include "TEmployeeHourly.h"

TEmployeeHourly::TEmployeeHourly(int _hour, const char* _name, ePosition _ePosition): TEmployee( _name, _ePosition)
{
	m_iWorkHour = _hour;
}


TEmployeeHourly::~TEmployeeHourly()
{
}

int TEmployeeHourly::GetMoney()
{
	int money = (m_iWorkHour * 10000) * (GetPosition()+1);
	return money;
}
