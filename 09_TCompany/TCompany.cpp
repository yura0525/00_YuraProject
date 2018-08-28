#include "TCompany.h"
#include <iostream>
using namespace std;

TCompany::TCompany(): m_iCountEmployee(0)
{
	
}

TCompany::~TCompany()
{
	for (int i = 0; i < MAX_EMPLOYEE; i++)
	{
		delete m_pEmployee[i];
	}
}

void TCompany::AddEmployee(TEmployee* _pEmployee)
{
	if (m_iCountEmployee < 0 || m_iCountEmployee > (MAX_EMPLOYEE-1))
	{
		printf("������ ������ �ʰ��Ͽ����ϴ�.\n");
	}
	m_pEmployee[m_iCountEmployee++] = _pEmployee;
}
void TCompany::PrintEmployeeMonthPay()
{
	printf("###########���� ���###########\n");
	printf("�̸�\t����\t�޿�\n");

	for (int i = 0; i < MAX_EMPLOYEE; i++)
	{
		printf("%s\t%s\t%d\n", m_pEmployee[i]->GetName(), m_pEmployee[i]->GetPositionString(), m_pEmployee[i]->GetMoney());
	}
}