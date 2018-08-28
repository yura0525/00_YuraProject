#pragma once
#include "TEmployee.h"
class TEmployeeHourly : public TEmployee
{
private:
	int m_iWorkHour;

public:
	TEmployeeHourly(int _hour, const char* _name,  ePosition _ePosition);
	~TEmployeeHourly();

	int GetMoney();
};

