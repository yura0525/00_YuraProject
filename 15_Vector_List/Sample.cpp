#include <vector>		//배열대체, 재할당
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <algorithm>
using namespace std;

class TStudent
{
public:
	int m_iIndex;
	string m_szName;
	int m_iType;
	TStudent(int iData)
	{
		m_iIndex = iData;
		m_iType = 0;
	}
	TStudent()
	{
		m_iIndex = 0;
		m_iType = 0;
	}
	TStudent(string name)
	{
		m_szName = name;
		m_iType = 1;
	}

	bool operator()(TStudent* data)
	{
		if(m_iType ==0)
			return (data->m_iIndex == m_iIndex);
		if (m_iType == 1)
			return (data->m_szName == m_szName);
		return false;
	}
	bool operator()(TStudent data)
	{
		if (m_iType == 0)
			return (data.m_iIndex == m_iIndex);
		if (m_iType == 1)
			return (data.m_szName == m_szName);
		return false;
	}
	bool operator<(TStudent* data)
	{
		if (m_iType == 0)
			return (data->m_iIndex < m_iIndex);
		//if (m_iType == 1)
		//	return (data->m_szName < m_szName);
		return false;
	}
};

TStudent* g_pData;
void Add(std::vector<TStudent*>& copy)
{
	for (int i = 0; i < 10; i++)
	{
		TStudent* pData = new TStudent(i);

		copy.push_back(pData);
		if (i == 5)
			g_pData = pData;
	}
}

void Add(std::list<TStudent*>& copy)
{
	for (int i = 0; i < 10; i++)
	{
		TStudent* pData = new TStudent(i);

		copy.push_back(pData);
		if (i == 5)
			g_pData = pData;
	}
}

void Delete(std::list<TStudent*>& copy)
{
	std::list<TStudent*>::reverse_iterator iter;
	for (iter = copy.rbegin(); iter != copy.rend(); iter++)
	{
		TStudent* data = (TStudent*)(*iter);
		delete data;
		copy.pop_back();
	}
}

void Delete(std::vector<TStudent*>& copy)
{
	for (int i = 9; i >= 0; i--)
	{
		TStudent* data = copy[i];
		delete data;
		copy.pop_back();
	}
}
template<typename T>
class TCheck
{
public:
	bool operator()(T data)
	{
		return (data->m_iIndex == m_iFind);
	}
	bool operator<(T data)
	{
		return (data->m_iIndex < m_iFind);
	}
	bool operator>(T data)
	{
		return (data->m_iIndex > m_iFind);
	}
	int m_iFind;
};

bool check(TStudent* a, TStudent* b)
{
	if (a->m_iIndex > b->m_iIndex)
		return true;

	return false;
}

bool check(TStudent* data)
{
	if (data->m_iIndex == 5)
		return true;

	return false;
}

int main()
{
	//############example1
	//std::vector<int> listData;
	//listData.push_back(10);
	//listData.push_back(3);
	//listData.push_back(5);
	//listData.push_back(12);
	//listData.push_back(1);
	//sort(listData.begin(), listData.end());

	//############example2
	//std::list<TStudent*> data;
	//Add(data);
	//
	//std::list<TStudent*>::iterator iter;
	//
	//iter = find(data.begin(), data.end(), g_pData);
	//iter = find_if(data.begin(), data.end(), check);			//find_if는 값을 가지고 비교한다.
	//
	//TCheck<TStudent*> checkPtr;
	//checkPtr.m_iFind = 5;
	//iter = find_if(data.begin(), data.end(), checkPtr);
	//
	//iter = find_if(data.begin(), data.end(), TStudent(5));
	//data.insert(iter, new TStudent(11));						//iter가 가리키는곳의 앞에 삽입
	//data.erase(data.begin(), data.end());
	//data.sort();

	//############example3
	//std::vector<TStudent*> data;
	//Add(data);

	//std::vector<TStudent*>::iterator iter;

	//TStudent* check = new TStudent(5);
	//iter = find(data.begin(), data.end(), g_pData);				//find는 포인터를 가지고 비교한다.
	//iter = find_if(data.begin(), data.end(), (*check));			//find_if는 값을 가지고 비교한다.

	//TCheck<TStudent*> checkPtr;
	//checkPtr.m_iFind = 5;
	//iter = find_if(data.begin(), data.end(), checkPtr);

	//iter = find_if(data.begin(), data.end(), TStudent(5));
	//
	//data.insert(iter, new TStudent(11));						//iter가 가리키는곳의 앞에 삽입

	//############example4
	//std::vector<int>::iterator iter;
	//std::vector<int> intArray;

	//intArray.reserve(10);//메모리확보		//capacity가 0이고 size는 0이다.

	//intArray.resize(10);	//메모리 재할당, 디폴트 생성자로 생성되어 들어간다.
	//intArray[0] = 10;		//intArray.reserve(10);만하고 바로 [0]으로 참조하면 out of range이다. resize하면 괜찮음. 메모리 재할당.
	//intArray[1] = 10;
	//intArray[2] = 10;

	//int ibyte = intArray.capacity();
	//intArray.resize(20);		//메모리 재할당.

	//int iFind = 5;

	//for (int i = 0; i < 10;i++)
	//{
	//	int a = i;
	//	intArray.push_back(a);	//resize(20);하고나서 push_back하면 21번째에 들어간다.
	//	
	//	if (iFind == i)
	//	{
	//		iter = intArray.begin();
	//	}
	//}
	//int a = 5;
	//iter = find(intArray.begin(), intArray.end(), a);
	//
	//intArray.insert(iter, a);
	//intArray.insert(intArray.begin(), 999);
	//intArray.insert(intArray.end(), 888);

	//error!!!
	//iter = find(intArray.begin(), intArray.end(), a);를 찾고, insert를 하면 iter는 다시 무효화.
	//무효화된 iter에 인서트 하려고 해서 에러
	//insert하기 직전에 iter = find(intArray.begin(), intArray.end(), 5);다시 찾으면 된다.
/*	a = 20;
	intArray.insert(iter, a);	*/	

	//for (iter = intArray.begin(); iter != intArray.end(); iter++)
	//{
	//	TStudent a = *iter;
	//	std::cout << a.m_iIndex << std::endl;
	//}

	//for (int i = 0; i < intArray.size(); i++)
	//{
	//	TStudent a = intArray[i];
	//	std::cout << a.m_iIndex << std::endl;
	//}

	//############example5
	/*std::vector<int> intArray1(10);
	std::vector<int> intArray2(10, 7);
	std::vector<int> intArray3(intArray1.begin(), intArray1.end());
	std::vector<int> intArray4;
	intArray4.assign(intArray2.begin(), intArray2.end());
	std::vector<int> intArray5;
	intArray5.assign(10, 9);

	std::vector<int>::iterator iter1 = find(intArray5.begin(), intArray5.end(), 5);

	if( iter1 != intArray5.end() )
		intArray5.erase(iter1);
	intArray5.clear();*/

	//############example6
	std::vector<TStudent>::iterator iter;
	std::vector<TStudent> intArray;

	//intArray.reserve(10);//메모리확보

	intArray.resize(10);	//메모리 재할당

	int ibyte = intArray.capacity();
	intArray.resize(20);

	int iFind = 5;

	for (int i = 0; i < 10; i++)
	{
		TStudent a;
		a.m_iIndex = i;
		intArray.push_back(a);

		if (iFind == i)
		{
			iter = intArray.begin();
		}
	}

	TStudent a;
	a.m_iIndex= 5;
	iter = find_if(intArray.begin(), intArray.end(), a);

	intArray.insert(iter, a);

	a.m_iIndex = 999;
	intArray.insert(intArray.begin(), a);

	a.m_iIndex = 888;
	intArray.insert(intArray.end(), a);

	//error!!!
	//iter = find(intArray.begin(), intArray.end(), a);를 찾고, insert를 하면 iter는 다시 무효화.
	//무효화된 iter에 인서트 하려고 해서 에러
	//insert하기 직전에 iter = find(intArray.begin(), intArray.end(), 5);다시 찾으면 된다.
	/*a.m_iIndex = 20;
	intArray.insert(iter, a);	*/			
	return 0;
}