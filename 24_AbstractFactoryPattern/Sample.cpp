//컴파일러를 개발해라!
//제품별 (sun , hp)
//Scanner, Parser, Generator, Optimizer
//클래스 구조, sun ? hp = 동일한 작업이 진행되어야한다.
#include <iostream>
#include <memory>
using namespace std;

#define WARRIOR		0 
#define MAGICIAN	1

//Product군
class TAttack	{};
class TDefense	{};

class TSword	: public TAttack{};
class TShield	: public TDefense {};

class TMagic			: public TAttack {};
class MagicianShield	: public TDefense {};

//Factory군
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

	//shared_ptr<TDefense> 생성자를 호출해서 복사한다. OK!!
	//pDefense = shared_ptr<TDefense>(g_pFactory->GetDefense());
	pDefense.reset(g_pFactory->GetDefense());

	//pDefense는 shared_ptr<TDefense>이고 .get()함수를 쓰면 TDefense*이 나온다.
	Print(pDefense.get());

	return 0;
}