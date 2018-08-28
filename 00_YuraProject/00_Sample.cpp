#include<stdio.h>
#include<conio.h>

void main()
{
	int diamondSize		= 0;

	printf("Diamond's Size : ");
	scanf_s("%d", &diamondSize);

	bool isDiamondUnder	= false;
	int starRowCount	= 0;
	int blankRowCount	= 0;
	bool isBlank		= false;

	for (int i = 0; i < diamondSize; i++)
	{
		isDiamondUnder = (i > (diamondSize / 2));
		starRowCount = (isDiamondUnder) ? (diamondSize - (2 * (i - (diamondSize / 2)))) : ((2 * i) + 1);
		blankRowCount = diamondSize - starRowCount;

		//printf(" isDiamondUnder = %s\n", ((isDiamondUnder) ? "TRUE" : "FALSE"));
		//printf(" starRowCount = %d , blankRowCount = %d\n ", starRowCount, blankRowCount);

		for (int j = 0; j < diamondSize; j++)
		{
			isBlank =(j < (blankRowCount / 2)) || (j >= (diamondSize - (blankRowCount / 2)));

			//printf(" isBlank = %s \n", ((isBlank) ? "TRUE" : "FALSE"));
			if (isBlank)
			{
				printf(" ");
			}
			else
			{
				printf("*");
			}
		}
		printf("\n");
	}

	getchar();
	getchar();
}