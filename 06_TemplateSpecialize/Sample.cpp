#include <conio.h>
#include <string.h>
#include <iostream>

using namespace std;

template<typename T>
int DataSize(T data)
{
	return sizeof(data);
}

template<>
int DataSize<>(const char* data)
{
	return strlen(data);
}
int main()
{
	int num = 0;
	double real = 0.0;

	const char* str = "Good morning";

	cout << DataSize(num) << endl;
	cout << DataSize(real) << endl;
	cout << DataSize(str) << endl;

	getchar();
	return 0;
}