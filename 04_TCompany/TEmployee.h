#pragma once
enum ePosition
{
	eSawon,
	eDaeri,
	eGuajang,
	eBujang,
	eEsa,
};

class TEmployee
{
private:
	char* m_Name;
	ePosition m_ePosition;
	
public:
	TEmployee(const char* _name, ePosition _ePosition );
	virtual ~TEmployee();

	inline char* GetName()
	{
		return m_Name;
	}
	inline ePosition GetPosition()
	{
		return m_ePosition;
	}

	const char* GetPositionString();
	virtual int GetMoney() = 0;
};


