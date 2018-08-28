#include <iostream>
#include <conio.h>

#include "TEmployeeHourly.h"
#include "TEmployeeMonthly.h"
#include "TEmployeeYearly.h"
#include "TCompany.h"

int main()
{
	TCompany company;

	company.AddEmployee(new TEmployeeHourly(200, "�ǻ��", ePosition::eSawon));
	company.AddEmployee(new TEmployeeHourly(220, "���μ�", ePosition::eSawon));
	company.AddEmployee(new TEmployeeMonthly(3, "���켺", ePosition::eDaeri));
	company.AddEmployee(new TEmployeeMonthly(5, "������", ePosition::eDaeri));
	company.AddEmployee(new TEmployeeYearly(3, "Ȳ����", ePosition::eGuajang));
	company.AddEmployee(new TEmployeeMonthly(15, "������", ePosition::eGuajang));
	company.AddEmployee(new TEmployeeMonthly(20, "�ֹν�", ePosition::eBujang));
	company.AddEmployee(new TEmployeeYearly(3, "������", ePosition::eBujang));
	company.AddEmployee(new TEmployeeHourly(50, "�ȼ���", ePosition::eEsa));
	company.AddEmployee(new TEmployeeYearly(4, "�۰�ȣ", ePosition::eEsa));

	company.PrintEmployeeMonthPay();

	getchar();
	return 0;
}