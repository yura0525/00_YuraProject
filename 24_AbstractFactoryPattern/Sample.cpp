//�����Ϸ��� �����ض�!
//��ǰ�� (sun , hp)
//Scanner, Parser, Generator, Optimizer
//Ŭ���� ����, sun ? hp = ������ �۾��� ����Ǿ���Ѵ�.
#include <iostream>
#include <memory>
using namespace std;

#define WARRIOR		0 
#define MAGICIAN	1

//Product��
class TAttack	{};
class TDefense	{};

class TSword	: public TAttack{};
class TShield	: public TDefense {};

class TMagic			: public TAttack {};
class MagicianShield	: public TDefense {};

//Factory��
class TCharacter
{
public:
	virtual TAttack*	GetAttack() = 0;
	virtual TDefense*	GetDefense() = 0;

	TAttack*	attack;
	TDefense*	defense;
	TCharacter()
	{
		attack = new TAttack;
		defense = new TDefense;
	}
};

class TWarrior : public TCharacter
{
public:
	TAttack * GetAttack() { return new TSword; }
	TDefense* GetDefense() { return new TShield; }
};

class TMagician : public TCharacter
{
public:
	TAttack * GetAttack()		{ return new TMagic; }
	TDefense* GetDefense()		{ return new MagicianShield; }
};

void Print(TDefense* data)
{
}

int g_iType = 0;	//Sun

class TClient
{

};
int main()
{
	g_iType = WARRIOR;

	//TCharacter* g_pFactory;
	//shared_ptr<TCharacter> data(new TWarrior());
	shared_ptr<TCharacter> g_pFactory;
	TCharacter * g_pFactory1;

	if (g_iType == WARRIOR)
	{
		g_pFactory1 = new TWarrior;
		g_pFactory = make_shared<TWarrior>();
	}
	else
	{
		g_pFactory1 = new TMagician;
		g_pFactory = make_shared<TMagician>();
	}

	/*shared_ptr<TAttack> pScanner(g_pFactory->GetAttack());
	shared_ptr<TDefense> pParser(g_pFactory->GetDefense());*/

	shared_ptr<TAttack> pAttack;
	//pAttack = std::move(g_pFactory->GetAttack());

	
	shared_ptr<TDefense> a(new TDefense);
	shared_ptr<TDefense> b(g_pFactory->GetDefense());
	//pDefense = std::move(a);		//OK
	//a.swap(b);

	auto temp = g_pFactory->GetDefense();
	shared_ptr<TDefense> pDefense(temp);
	//unique_ptr<TDefense> pDefense2;

	//shared_ptr<TDefense> �����ڸ� ȣ���ؼ� �����Ѵ�. OK!!
	//pDefense = shared_ptr<TDefense>(g_pFactory->GetDefense());
	pDefense.reset(g_pFactory->GetDefense());

	//pDefense�� shared_ptr<TDefense>�̰� .get()�Լ��� ���� TDefense*�� ���´�.
	Print(pDefense.get());

	return 0;
}