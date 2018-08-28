#include "TToken.h"
#include <conio.h>
void main()
{
	char buffer[] = "kgca game academy";
	//TValue value(buffer);
	//TToken token1;
	//token1.Show();
	//array[0] = kgca
	//array[1] = game
	//array[2] = academy

	TToken token(buffer);
	token.Show();
	getchar();
}