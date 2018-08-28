#pragma once
#include <string>

template<typename YURA>
void Swap(YURA* a, YURA* b)
{
	YURA* temp	= new YURA;
	*temp		= *a;
	*a			= *b;
	*b			= *temp;

	delete temp;
}

template<>
void Swap<>(char* a, char* b)
{
	size_t size = strlen(a) + 1;
	char* temp = new char[size];

	strcpy(temp, a);
	strcpy(a, b);
	strcpy(b, temp);

	delete temp;
}




