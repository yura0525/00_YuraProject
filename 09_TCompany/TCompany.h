#pragma once
#include "TEmployee.h"
#define MAX_EMPLOYEE 10
class TCompany
{
private:
	TEmployee*	m_pEmployee[MAX_EMPLOYEE];
	int			m_iCountEmployee;
public:
	TCompany();
	~TCompany();
	void AddEmployee(TEmployee* _pEmployee);
	void PrintEmployeeMonthPay();
};

