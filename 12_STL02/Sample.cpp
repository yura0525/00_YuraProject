#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>
#include <functional>		//함수 객체, bind2nd

using namespace std;

class TData
{
public:
	int a;
	int b;
	int c;
};
class TStudent
{
public:
	int m_iValue;
	TStudent(int i): m_iValue(i){}

	void print() const
	{
		cout << m_iValue << endl;
	}
	void printWithString(TData data) const
	{
		cout<< m_iValue <<" " << data.a << endl;
	}
};

void PRINTER(const TStudent* pData)
{
	cout << pData->m_iValue;
}
void PRINTER(int iData)
{
	cout << iData;
}
//void PRINTER(const TStudent& pData)
//{
//	cout << pData.m_iValue;
//}

void SetData(std::list<int>& coll)
{
	for (int i = 0; i < 5; i++)
	{
		coll.push_back(i);
	}
}
class intData
{
public:
	int m_iValue;
	intData(int value) : m_iValue(value){}
	int operator()()
	{
		return m_iValue += 2;
	}
};

int setData()
{
	static int m_iValue = 0;
	return m_iValue ++;
}

void getData(int value)
{
	cout << value;
}
int main()
{
	///////////////////////////////map///////////////////////////////////////////////
	//사전식 자료관리
	//Pair = 키(인덱스) + 값
	//Key 인덱스가 유일해야한다.
	//데이터가 많을 경우 검색이 빠르다.
	//make_pair로 페어를 만든다.
	//검색을 빠르게 하기 위해서 존재한다.(mapList1.find(100);)

	///////////////////////example 1
	/*std::map<int, int> mapList1;
	for (int i = 0; i < 10000; i++)
	{
		mapList1.insert(make_pair(i, rand() % 1000));
	}
	
	for (map<int, int>::iterator iter = mapList1.begin(); iter != mapList1.end(); iter++)
	{
		int iKey = iter->first;
		int iValue = iter->second;

		cout << "[" << iKey << "] " <<iValue << endl;
	}
	map<int, int>::iterator iter;
	iter = mapList1.find(100);
	if (iter != mapList1.end())
	{
		int iKey = iter->first;
		int iValue = iter->second;

		cout << "[" << iKey << "] " << iValue << endl;
	}
	for (int iCnt = 0; iCnt<mapList1.size();iCnt++)
	{
		cout << mapList1[iCnt] << endl;
	}*/

	///////////////////////example 2
	//std::map<int, string> mapList;
	//mapList.insert(make_pair(1, "홍길동"));

	//mapList[100] = "홍길동";
	//mapList[10] = "김유라";

	//cout << mapList[0] << endl;
	//cout << mapList[1] << endl;

	
	///////////////////////////////list//////////////////////////////////////////////
	

	///////////////////////example 3
	//std::list<int> coll;

	////generate, generate_n return값이 있어야한다.
	////coll안에 10개의 리스트가 들어간다.
	////9,8,7,6,5,4,3,2,1,0으로 들어간다.
	////setData안의 리턴값이 0부터 1씩증가된다.
	//generate_n(front_inserter(coll), 10, setData);
	//for (list<int>::iterator iter = coll.begin(); iter != coll.end(); iter++)
	//{
	//	cout << *iter << endl;
	//}

	////coll리스트 안에 setData안의 리턴값이 10(16진수 0x0a)부터 19(0x13)으로 10개 채워진다.
	//generate(coll.begin(), coll.end(), setData);
	//for (list<int>::iterator iter = coll.begin(); iter != coll.end(); iter++)
	//{
	//	cout << *iter << endl;
	//}

	////coll리스트 index 10번째부터 14번째까지 5개 채워진다.
	////3,5,6,9, 0xb로 채워진다. 리스트는 15개이다.
	//generate_n(back_inserter(coll), 5,intData(1));
	//for (list<int>::iterator iter = coll.begin(); iter != coll.end(); iter++)
	//{
	//	cout << *iter << endl;
	//}

	////coll리스트는 15번째부터 19번째까지 5개 리스트가 채워진다.
	////0,1,2,3,4,5 가 뒤에 추가된다. 리스트 갯수는 20개이다.
	//SetData(coll);
	//for (list<int>::iterator iter = coll.begin(); iter != coll.end(); iter++)
	//{
	//	cout << *iter << endl;
	//}

	///////////////////////example 4
	//들어있는 객체가 포인터(TStudent*)이면 mem_func이고, 객체 자체(TStudent)이면 mem_func_ref를 붙인다.
	//set은 TStudent 자체를 저장하지 말고, TStudent*를 저장하게하자.

	//셋에서 구조체를 사용시는 비교연산자를 제공해야 정렬할 수 있다.
	////mrefList.insert(b);								error!!!!!!! 
	//구조체를 사용할때는  bool operator<(const TData& lhs, const TData& rhs) 함수를 제공해야 삽입할수있다.
	//TStudent* 포인터변수는 포인터 주소값으로 정렬하기 때문에 그냥 삽입할수있다.

	//multiset<TStudent> mrefList;

	//TStudent a(7);
	//TStudent b(9);
	//mrefList.insert(a);
	//mrefList.insert(b);

	//for_each(mrefList.begin(), mrefList.end(), PRINTER);	//멤버함수
	//for_each(mrefList.begin(), mrefList.end(), mem_func_ref(&TStudent::print));	//멤버함수

	//TData data;
	//data.a = 9;
	////for_each(mrefList.begin(), mrefList.end(), bind2nd(mem_func_ref(&TStudent::printWithString), data));


	///////////////////////example 5
	////중복 데이터 입력 가능, 정렬이 되어 입력
	//multiset<TStudent*> multiSetList;

	//multiSetList.insert(new TStudent(3));
	//multiSetList.insert(new TStudent(3));
	//multiSetList.insert(new TStudent(1));
	//multiSetList.insert(new TStudent(2));
	//multiSetList.insert(new TStudent(2));
	//multiSetList.insert(new TStudent(3));
	//multiSetList.insert(new TStudent(3));

	//int imCnt = multiSetList.size();
	//cout << "multiSetList = " << imCnt << endl;			//사이즈는 7이다.

	////둘이 같다
	////bind2nd, 바인드는 인자값 1개를 바인드한다.
	////mem_fun 멤버 함수일경우에 쓴다.
	//for_each(multiSetList.begin(), multiSetList.end(), PRINTER);		//전역함수	2,3,3,3,1,2,3
	//cout << endl;
	//for_each(multiSetList.begin(), multiSetList.end(), mem_fun(&TStudent::print));	//멤버함수	2,3,3,3,1,2,3

	//TData data;
	//data.a = 9;
	//for_each(multiSetList.begin(), multiSetList.end(), bind2nd(mem_fun(&TStudent::printWithString), data));//전달인자 있는 멤버함수

	//int g_iCnt = 0;
	//for (multiset<TStudent*>::iterator iter = multiSetList.begin(); iter != multiSetList.end(); iter++)
	//{
	//	int iValue = (*iter)->m_iValue;
	//	cout << "["<<g_iCnt++<<"] = "<<iValue << " ";
	//}
	//cout << endl << endl;;

	////값이 2인 TStudent객체를 지우기.
	//g_iCnt = 0;
	//for (multiset<TStudent*>::iterator iter = multiSetList.begin(); iter != multiSetList.end(); iter++)
	//{
	//	int iValue = (*iter)->m_iValue;
	//	if (iValue == 2)
	//	{
	//		cout << "[" << g_iCnt++ << "] = " << iValue << " ";
	//		iter = multiSetList.erase(iter);
	//		if (iter == multiSetList.end())
	//			break;
	//	}
	//	
	//}

	//for (multiset<TStudent*>::iterator iter = multiSetList.begin(); iter != multiSetList.end(); iter++)
	//{
	//	int iValue = (*iter)->m_iValue;
	//	cout << "[" << g_iCnt++ << "] = " << iValue << " ";
	//}

	//multiSetList.clear();


	///////////////////////example 6
	////중복 데이터 입력 불가
	////입력된 종류와 종류의 가짓수를 찾는데 사용한다.
	//set<int> setList;
	//setList.insert(1);
	//setList.insert(2);
	//setList.insert(2);
	//setList.insert(3);
	//setList.insert(3);

	//int iCnt = setList.size();			//1,2,2,3,3 5개 입력했어도 중복데이터 입력불가이므로 3출력
	//int i2Cnt = setList.count(2);		//중복데이터 입력 불가이므로 무조건 1가지씩 들어간다. 1출력
	//cout << "\nsetListSize = " << iCnt << " 2'sCount " << i2Cnt << endl;

	//for_each(setList.begin(), setList.end(), PRINTER);
	//for (set<int>::iterator iter = setList.begin(); iter != setList.end(); iter++)
	//{
	//	int iValue = *iter;
	//	cout << iValue;
	//}

	////crbegin 콘스트리버스 이터레이터
	//for (set<int>::const_reverse_iterator iter = setList.crbegin(); iter != setList.crend(); iter++)
	//{
	//	PRINTER(*iter);
	//}

	//setList.erase(2);
	//for (set<int>::iterator iter = setList.begin(); iter != setList.end(); iter++)
	//{
	//	int iValue = *iter;
	//	cout << iValue;
	//}
	//setList.clear();
	return 0;
}
