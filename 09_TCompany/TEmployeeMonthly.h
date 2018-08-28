#pragma once
#include "TEmployee.h"
class TEmployeeMonthly : public TEmployee
{
private:
	int m_iYear;
public:
	TEmployeeMonthly(int _iYear, const char* _Name, ePosition _Position);
	~TEmployeeMonthly();

	int GetMoney();
};

