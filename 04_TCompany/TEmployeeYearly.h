#pragma once
#include "TEmployee.h"
class TEmployeeYearly : public TEmployee
{
private:
	int m_iGrade;
public:
	TEmployeeYearly(int _iGrade, const char* _name, ePosition _ePosition);
	~TEmployeeYearly();

	int GetMoney();
};

