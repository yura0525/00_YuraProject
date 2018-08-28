#include "TStack.cpp"
#include <iostream>
#include <stack>

using namespace std;
void PRINT(int iValue)
{
	std::cout << iValue << " ";
}

class TStudent
{
public:
	int m_iID;
	TStudent(int iData)
	{
		m_iID = iData;
	}
	TStudent()
	{
		m_iID = -1;
	}
	bool operator==(TStudent* src)
	{
		if ((src != NULL) && (m_iID == src->m_iID))
			return true;

		return false;
	}
};

void PRINT(TStudent* iValue)
{
	if (iValue != NULL)
	{
		std::cout << iValue->m_iID << " ";
		return;
	}
	
}
int main()
{
	//stack을 배열로 구현해보자. 
	//int형으로 했다가 템플릿클래스로 구현하자.
	//TStudent로 했다가 TStudent*로 했다가 stl의 stack으로 해보자.
	std::stack<TStudent *>list;

	list.push(new TStudent(10));
	list.push(new TStudent(11));
	list.push(new TStudent(12));
	list.push(new TStudent(13));
	list.push(new TStudent(14));

	TStudent* pA = list.top();
	list.pop();
	TStudent* pB = list.top();
	list.pop();
	TStudent* pC = list.top();
	list.pop();
	TStudent* pD = list.top();
	list.pop();
	TStudent* pE = list.top();
	list.pop();

	if (list.empty())
	{
		cout << "NULL OBJECT" << endl;
	}

	//TStudent* 형
	/*TStack<TStudent*> stack;
	TStudent a(0), b(1), c(2), d(3);

	PRINT(stack.Push(&a));
	PRINT(stack.Push(&b));
	PRINT(stack.Push(&c));
	if (stack.Push(&d) == NULL)
	{
		cout << "main() OverFlow" << endl;
	}

	stack.Show();

	PRINT(stack.Pop());
	PRINT(stack.Pop());
	PRINT(stack.Pop());

	if (stack.Pop() == NULL)
	{
		cout << "main() UnderFlow" << endl;
	}

	if (stack.Pop() == NULL)
	{
		cout << "main() UnderFlow" << endl;
	}

	TStudent* temp1 = stack.Pop();
	TStudent* temp2 = TStack<TStudent*>::g_NULL;
	if (temp1 == NULL && (temp1 == temp2))
	{
		cout << "NULL OBJECT" << endl;
	}*/

	//TStudent형
	/*TStack<TStudent> stack;
	TStudent a(0), b(1), c(2),d(3);

	PRINT(stack.Push(a).m_iID);
	PRINT(stack.Push(b).m_iID);
	PRINT(stack.Push(c).m_iID);
	PRINT(stack.Push(d).m_iID);
	stack.Show();

	PRINT(stack.Pop().m_iID);

	PRINT(stack.Pop().m_iID);
	PRINT(stack.Pop().m_iID);
	PRINT(stack.Pop().m_iID);
	PRINT(stack.Pop().m_iID);

	TStudent& temp1 = stack.Pop();
	TStudent& temp2 = TStack<TStudent>::g_NULL;
	if (temp1 == TStack<TStudent>::g_NULL)
	{
		cout << "NULL OBJECT" << endl;
	}*/

	//int 형
	/*TStack<int> stack;
	PRINT(stack.Push(0));
	PRINT(stack.Push(1)); 
	PRINT(stack.Push(2)); 
	PRINT(stack.Push(3)); 
	stack.Show();

	PRINT(stack.Pop());
	PRINT(stack.Push(4));

	PRINT(stack.Pop());
	PRINT(stack.Pop());
	PRINT(stack.Pop());
	PRINT(stack.Pop());
*/
	return 0;
}