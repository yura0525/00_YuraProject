#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>
#include <functional>		//�Լ� ��ü, bind2nd

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
	//������ �ڷ����
	//Pair = Ű(�ε���) + ��
	//Key �ε����� �����ؾ��Ѵ�.
	//�����Ͱ� ���� ��� �˻��� ������.
	//make_pair�� �� �����.
	//�˻��� ������ �ϱ� ���ؼ� �����Ѵ�.(mapList1.find(100);)

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
	//mapList.insert(make_pair(1, "ȫ�浿"));

	//mapList[100] = "ȫ�浿";
	//mapList[10] = "������";

	//cout << mapList[0] << endl;
	//cout << mapList[1] << endl;

	
	///////////////////////////////list//////////////////////////////////////////////
	

	///////////////////////example 3
	//std::list<int> coll;

	////generate, generate_n return���� �־���Ѵ�.
	////coll�ȿ� 10���� ����Ʈ�� ����.
	////9,8,7,6,5,4,3,2,1,0���� ����.
	////setData���� ���ϰ��� 0���� 1�������ȴ�.
	//generate_n(front_inserter(coll), 10, setData);
	//for (list<int>::iterator iter = coll.begin(); iter != coll.end(); iter++)
	//{
	//	cout << *iter << endl;
	//}

	////coll����Ʈ �ȿ� setData���� ���ϰ��� 10(16���� 0x0a)���� 19(0x13)���� 10�� ä������.
	//generate(coll.begin(), coll.end(), setData);
	//for (list<int>::iterator iter = coll.begin(); iter != coll.end(); iter++)
	//{
	//	cout << *iter << endl;
	//}

	////coll����Ʈ index 10��°���� 14��°���� 5�� ä������.
	////3,5,6,9, 0xb�� ä������. ����Ʈ�� 15���̴�.
	//generate_n(back_inserter(coll), 5,intData(1));
	//for (list<int>::iterator iter = coll.begin(); iter != coll.end(); iter++)
	//{
	//	cout << *iter << endl;
	//}

	////coll����Ʈ�� 15��°���� 19��°���� 5�� ����Ʈ�� ä������.
	////0,1,2,3,4,5 �� �ڿ� �߰��ȴ�. ����Ʈ ������ 20���̴�.
	//SetData(coll);
	//for (list<int>::iterator iter = coll.begin(); iter != coll.end(); iter++)
	//{
	//	cout << *iter << endl;
	//}

	///////////////////////example 4
	//����ִ� ��ü�� ������(TStudent*)�̸� mem_func�̰�, ��ü ��ü(TStudent)�̸� mem_func_ref�� ���δ�.
	//set�� TStudent ��ü�� �������� ����, TStudent*�� �����ϰ�����.

	//�¿��� ����ü�� ���ô� �񱳿����ڸ� �����ؾ� ������ �� �ִ�.
	////mrefList.insert(b);								error!!!!!!! 
	//����ü�� ����Ҷ���  bool operator<(const TData& lhs, const TData& rhs) �Լ��� �����ؾ� �����Ҽ��ִ�.
	//TStudent* �����ͺ����� ������ �ּҰ����� �����ϱ� ������ �׳� �����Ҽ��ִ�.

	//multiset<TStudent> mrefList;

	//TStudent a(7);
	//TStudent b(9);
	//mrefList.insert(a);
	//mrefList.insert(b);

	//for_each(mrefList.begin(), mrefList.end(), PRINTER);	//����Լ�
	//for_each(mrefList.begin(), mrefList.end(), mem_func_ref(&TStudent::print));	//����Լ�

	//TData data;
	//data.a = 9;
	////for_each(mrefList.begin(), mrefList.end(), bind2nd(mem_func_ref(&TStudent::printWithString), data));


	///////////////////////example 5
	////�ߺ� ������ �Է� ����, ������ �Ǿ� �Է�
	//multiset<TStudent*> multiSetList;

	//multiSetList.insert(new TStudent(3));
	//multiSetList.insert(new TStudent(3));
	//multiSetList.insert(new TStudent(1));
	//multiSetList.insert(new TStudent(2));
	//multiSetList.insert(new TStudent(2));
	//multiSetList.insert(new TStudent(3));
	//multiSetList.insert(new TStudent(3));

	//int imCnt = multiSetList.size();
	//cout << "multiSetList = " << imCnt << endl;			//������� 7�̴�.

	////���� ����
	////bind2nd, ���ε�� ���ڰ� 1���� ���ε��Ѵ�.
	////mem_fun ��� �Լ��ϰ�쿡 ����.
	//for_each(multiSetList.begin(), multiSetList.end(), PRINTER);		//�����Լ�	2,3,3,3,1,2,3
	//cout << endl;
	//for_each(multiSetList.begin(), multiSetList.end(), mem_fun(&TStudent::print));	//����Լ�	2,3,3,3,1,2,3

	//TData data;
	//data.a = 9;
	//for_each(multiSetList.begin(), multiSetList.end(), bind2nd(mem_fun(&TStudent::printWithString), data));//�������� �ִ� ����Լ�

	//int g_iCnt = 0;
	//for (multiset<TStudent*>::iterator iter = multiSetList.begin(); iter != multiSetList.end(); iter++)
	//{
	//	int iValue = (*iter)->m_iValue;
	//	cout << "["<<g_iCnt++<<"] = "<<iValue << " ";
	//}
	//cout << endl << endl;;

	////���� 2�� TStudent��ü�� �����.
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
	////�ߺ� ������ �Է� �Ұ�
	////�Էµ� ������ ������ �������� ã�µ� ����Ѵ�.
	//set<int> setList;
	//setList.insert(1);
	//setList.insert(2);
	//setList.insert(2);
	//setList.insert(3);
	//setList.insert(3);

	//int iCnt = setList.size();			//1,2,2,3,3 5�� �Է��߾ �ߺ������� �ԷºҰ��̹Ƿ� 3���
	//int i2Cnt = setList.count(2);		//�ߺ������� �Է� �Ұ��̹Ƿ� ������ 1������ ����. 1���
	//cout << "\nsetListSize = " << iCnt << " 2'sCount " << i2Cnt << endl;

	//for_each(setList.begin(), setList.end(), PRINTER);
	//for (set<int>::iterator iter = setList.begin(); iter != setList.end(); iter++)
	//{
	//	int iValue = *iter;
	//	cout << iValue;
	//}

	////crbegin �ܽ�Ʈ������ ���ͷ�����
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
