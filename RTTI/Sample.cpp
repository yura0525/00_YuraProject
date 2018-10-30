#include <iostream>
#include <typeinfo.h>
#include <queue>
#include <vector>
//-------------------------------------------------------------------------------------------
//라이브러리부분.
#define STRING(s) #s		//# 스트링화 연산자
#define LINK(i, j) i##j		//##연결연산자

class CBase {};
class CChild : public CBase {};


#define DECLARE_CLASSNAME static char lpszclassName[];
#define IMPLEMENT_CLASSNAME(s) char s##::lpszclassName[] = #s;

#define DECLARE_DYNCREATE(s) static CObject* CreateObject();
#define IMPLEMENT_DYNCREATE(s) CObject* s::CreateObject()	{return new s;}

#define DECLARE_DYNAMIC(s) static CRuntimeClass class##s;
#define IMPLEMENT_DYNAMIC(s)	CRuntimeClass	s::class##s = { #s, sizeof(s), s::CreateObject };

#define DECLARE_DYNCLASS_CREATE CRuntimeClass* GetRuntimeClass() const;
#define IMPLEMENT_DYNCLASS_CREATE(s) CRuntimeClass* s::GetRuntimeClass() const{return &class##s;}

#define RUNTIME_CLASS(s) &s::class##s;

//&(CUser::classCUser);
class CObject;
struct CRuntimeClass
{
	char m_lpszClassName[21];
	int m_iObjectSize;
	CObject* (*ptrCreateObject)();
	CObject* CreateObject()
	{
		return (*ptrCreateObject)();
	}
};

class CObject
{
public:
	//static CRuntimeClass classCObject;
	DECLARE_DYNAMIC(CObject);	
	
	DECLARE_CLASSNAME;

	virtual char* GetClassName() const
	{
		return NULL;
	}
	virtual ~CObject() {}
	static CObject* CreateObject();
	//단독 사용불가. 파생해서 쓰게한다.
protected:
	CObject() {}
};

CObject* CObject::CreateObject() { return NULL; }
IMPLEMENT_CLASSNAME(CObject);

//CObject의 함수포인터는 NULL이다.
//CRuntimeClass	CObject::classCObject = { "CObject", sizeof(CObject), NULL };
IMPLEMENT_DYNAMIC(CObject);


class TSessionMgr
{
public:
	std::queue<CObject*> m_StaticQueue;
	std::vector<CObject*> m_UserList;
	void Create(CRuntimeClass* pRTC)
	{
		for (int iQueue = 0; iQueue < 1000; iQueue++)
		{
			CObject* pObj = pRTC->CreateObject();
			m_StaticQueue.push(pObj);
		}
	}
	CObject* CreateNewSession()
	{
		CObject* newSession = (CObject*)m_StaticQueue.front();
		m_UserList.push_back(newSession);
		return newSession;
	}

	CObject* RemoveNewSession(CObject* pSession)
	{
		std::vector<CObject*>::iterator iter;
		for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
		{
			if (*iter == pSession)
			{
				m_UserList.erase(iter);
				break;
			}
		}
		m_StaticQueue.push(pSession);
		return pSession;
	}
};

TSessionMgr g_SessionMgr;
//----------------------------------------------------------------------------------------

//우리가 코딩하는 부분.
class CMyClass : public CObject
{
public:
	DECLARE_CLASSNAME;

	//static CRuntimeClass classCMyClass;
	DECLARE_DYNAMIC(CMyClass);

	// static CObject* CreateObject();
	DECLARE_DYNCREATE(CMyClass);

	DECLARE_DYNCLASS_CREATE;
	//virtual char* GetClassName() const	{	return NULL;	}
};
//IMPLEMENT_CLASSNAME(CMyClass);
//CObject* CMyClass::CreateObject(){	return new CMyClass;	}

IMPLEMENT_DYNCREATE(CMyClass);
IMPLEMENT_DYNAMIC(CMyClass);
IMPLEMENT_CLASSNAME(CMyClass);
IMPLEMENT_DYNCLASS_CREATE(CMyClass);


class DChild : public CChild {};
//RTTI => Run Time Type Information : 실시간 타입 정보 얻기
void main()
{
#pragma region LINK&STRING
	std::cout << STRING(kgca) << std::endl;

	char m_szValue[] = "kgcagame";
	std::cout << LINK(m_sz, Value) << std::endl;
	std::cout << m_szValue << std::endl;

	int LINK(m_i,Data) = 0;
	//int m_iData = 0;
#pragma endregion

#pragma region aaa
	CBase parent;
	CChild child;

	const type_info& tBase = typeid(parent);
	const type_info& tChild = typeid(child);

	std::cout << tBase.name() <<"{ }"<< std::endl;
	std::cout << typeid(parent).name() << std::endl;

	std::cout << tChild.name() << std::endl;
	std::cout << typeid(child).name() << std::endl;

	std::cout << tBase.before(tChild) << std::endl;
	std::cout << tChild.before(tBase) << std::endl;
	std::cout << (tChild == tBase) << std::endl;
#pragma endregion

	CRuntimeClass* pRTCMyClass = RUNTIME_CLASS(CMyClass);
	g_SessionMgr.Create(pRTCMyClass);
	//CObject* p = pRTCMyClass->CreateObject();
	//CMyClass* pChild = (CMyClass*)p;
	//std::cout << pChild->lpszclassName;
	//delete p;


}
