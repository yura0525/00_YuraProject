#include <iostream>
#include <conio.h>

#include "TEmployeeHourly.h"
#include "TEmployeeMonthly.h"
#include "TEmployeeYearly.h"
#include "TCompany.h"

int main()
{
	TCompany company;

	company.AddEmployee(new TEmployeeHourly(200, "권상우", ePosition::eSawon));
	company.AddEmployee(new TEmployeeHourly(220, "조인성", ePosition::eSawon));
	company.AddEmployee(new TEmployeeMonthly(3, "감우성", ePosition::eDaeri));
	company.AddEmployee(new TEmployeeMonthly(5, "강동원", ePosition::eDaeri));
	company.AddEmployee(new TEmployeeYearly(3, "황정민", ePosition::eGuajang));
	company.AddEmployee(new TEmployeeMonthly(15, "박중훈", ePosition::eGuajang));
	company.AddEmployee(new TEmployeeMonthly(20, "최민식", ePosition::eBujang));
	company.AddEmployee(new TEmployeeYearly(3, "정진영", ePosition::eBujang));
	company.AddEmployee(new TEmployeeHourly(50, "안성기", ePosition::eEsa));
	company.AddEmployee(new TEmployeeYearly(4, "송강호", ePosition::eEsa));

	company.PrintEmployeeMonthPay();

	getchar();
	return 0;
}