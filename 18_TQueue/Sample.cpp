#include <iostream>
#include "TQueue.h"
#include <queue>
#include <deque>

int main()
{
	//stl::deque, 이중연결리스트와 동일하다
	std::deque<int> dequeList;
	dequeList.push_back(0);
	dequeList.push_back(1);
	dequeList.push_back(2);

	int iA = dequeList.front();
	int iB = dequeList.back();

	dequeList.pop_front();
	dequeList.pop_front();

	//stl::Quque
	/*std::queue<int> queueList;
	queueList.push(0);
	queueList.push(1);
	queueList.push(2);

	int iA = queueList.front();
	int iB = queueList.back();

	queueList.pop();*/

	//만든 Queue
	/*TQueue queue;
	queue.Put(0);
	queue.Put(1);
	queue.Put(2);
	queue.Put(3);
	queue.Put(4);
		queue.Put(5);

	queue.Get();
	queue.Put(6);
	queue.Put(7);

	queue.Get();
	queue.Get();
	queue.Get();
	queue.Get();
	queue.Get();*/
	return 0;
}