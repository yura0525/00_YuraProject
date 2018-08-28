#include <iostream>
#include <vector>
using namespace std;

//��ü�� �����ϴ� �κ��� ���� �����ϰ�
//�̸� �����Ͽ� ��ü�� �ϼ��Ѵ�.
class Product	{};
class TAttack	: public Product {};
class TDefense	: public Product {};

class TCharacter
{
private:
	Product* product[2];

public:
	virtual void BuildAttack() = 0;
	virtual void BuildDefense() = 0;
	virtual TCharacter* GetCharacter() = 0;
};

class TWarrior : public TCharacter
{
public:
	void BuildAttack() {}
	void BuildDefense() {}
	TCharacter* GetCharacter() { return this; }
};

class Magician : public TCharacter
{
public:
	void BuildAttack() {}
	void BuildDefense() {}
	TCharacter* GetCharacter() { return this; }
};


class TDirector
{
public:
	
	TCharacter* CreateCharacter(TCharacter& pBuilder)
	{
		pBuilder.BuildAttack();
		pBuilder.BuildDefense();
		//pBuilder.BuildRoom();
		//pBuilder.BuildDoor();
		//pBuilder.BuildWall();

		return pBuilder.GetCharacter();
	};

};
int main()
{
	TDirector director;
	TWarrior build;
	TCharacter* character = director.CreateCharacter(build);

	return 0;
}