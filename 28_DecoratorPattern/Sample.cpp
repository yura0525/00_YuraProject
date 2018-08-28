#include <iostream>
#include <vector>
using namespace std;

class AirPlane
{
public:
	virtual void Attack() = 0;
};

class FrontAttackAirPlane : public AirPlane
{
public:
	void Attack()
	{
		cout << "�������" << endl;
	}
};

class Decorator : public AirPlane
{
public:
	Decorator(AirPlane* pAirPlane)
	{
		m_AttackList.push_back(pAirPlane);
	}
	void Attack()
	{
		for (int iAttack = 0; iAttack < m_AttackList.size(); iAttack++)
		{
			m_AttackList[iAttack]->Attack();
		}
	}
	void AddAttack(AirPlane* pAirPlane)
	{
		m_AttackList.push_back(pAirPlane);
	}

	void DelAttack(AirPlane* pAirPlane)
	{
		vector<AirPlane *>::iterator iter;
		for(iter = m_AttackList.begin(); iter != m_AttackList.end(); iter++)
		{
			AirPlane* pAir = (*iter);

			if (pAir == pAirPlane)
			{
				m_AttackList.erase(iter);
				break;
			}
		}
	}
	
protected:
	vector<AirPlane *> m_AttackList;
};

class SideAttackAirPlane : public Decorator
{
public:
	
	SideAttackAirPlane(AirPlane* pAirPlane) : Decorator(pAirPlane)
	{
	}
	void Attack()
	{
		Decorator::Attack();
		cout << "�������" << endl;
	} 
};

class RearSideAttackAirPlane : public Decorator
{
public:
	RearSideAttackAirPlane(AirPlane* pAirPlane) : Decorator(pAirPlane)
	{
	}
	void Attack()
	{
		Decorator::Attack();
		cout << "�Ĺ����" << endl;
	}
};

class User
{
public:
	AirPlane * m_pAttack;
	User()
	{
		m_pAttack = 0;
	}
	void DeleteAttack()
	{
		delete m_pAttack;
		m_pAttack = 0;
	}
	void SetAttack(AirPlane* pAirPlane)
	{
		m_pAttack = pAirPlane;
	}
	void AddAttack(AirPlane* pAirPlane)
	{
		Decorator* pAdd = dynamic_cast<Decorator*>(m_pAttack);
		if(pAdd != NULL)
			pAdd->AddAttack(pAirPlane);
	}

	void DelAttack(AirPlane* pAirPlane)
	{
		Decorator* pAdd = dynamic_cast<Decorator*>(m_pAttack);
		if (pAdd != NULL)
			pAdd->DelAttack(pAirPlane);
	}

	void AttackAction()
	{
		m_pAttack->Attack();
	}
};




int main()
{
	//����0
	AirPlane* userFront = new FrontAttackAirPlane;
	//����1
	AirPlane* userSide = new SideAttackAirPlane(userFront);
	//����2
	AirPlane* rearSide = new RearSideAttackAirPlane(userSide);

	//�������� �����ؼ� �������� �������.
	//���� ����, �������� ����Ѵ�.
	User user;
	user.SetAttack(userFront);
	user.AttackAction();

	user.SetAttack(userSide);
	user.AddAttack(userFront);
	user.DelAttack(userFront);
	user.AttackAction();

	user.SetAttack(rearSide);
	user.AttackAction();

	delete rearSide;
	delete userSide;
	delete userFront;

	//������ ���
	//��������, ���� ����ִ�.
	/*User user;
	user.SetAttack(new FrontAttackAirPlane);
	user.AttackAction();
	user.DeleteAttack();

	user.SetAttack(new SideAttackAirPlane(new FrontAttackAirPlane));
	user.AttackAction();
	user.DeleteAttack();

	user.SetAttack(new RearSideAttackAirPlane(new SideAttackAirPlane(new FrontAttackAirPlane)));
	user.AttackAction();
	user.DeleteAttack();*/

	return 0;
}