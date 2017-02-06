#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int MAX_LEN = 3;
int SynErr = 0;
int DivErr = 0;
int SqrtErr = 0;
int LowPrec = 0;

/*
Storing with least significant digit in front of the array
*/
typedef struct
{
	int* list;
	int len_decimal;
	int len_total;
	int is_neg;
} bigint;

bigint* conv_str_to_bigint(int is_neg,char* string)
{
	int len_str = strlen(string);
	bigint* big = (bigint*) calloc(1,sizeof(bigint));
	big->len_total = len_str;
	big->list = (int*)calloc(MAX_LEN,sizeof(int));	//Initializes to zero
	big->is_neg = is_neg;
	int i=len_str-1;
	int j=0;
	for(;i>=0;i--)
	{
		if(string[i] != '.')
		{
			big->list[j] = (int)(string[i] - '0');
			j++;
		}
		else
		{
			big->len_decimal = len_str - i - 1;
			big->len_total--;
		}
	}
	return big;
}

int reduce(bigint* big)
{
	if(big->len_total > MAX_LEN)
	{
		if((big->len_total - big->len_decimal) > MAX_LEN)
		{
			LowPrec = 1;
			return -1;
		}
		else
		{
			//TODO: Round Off Procedure
			int* new_list = (int *) calloc(MAX_LEN,sizeof(int));
			int i= big->len_total - MAX_LEN;
			int j=0;
			for(;i<big->len_total;i++)
			{
				new_list[j] = big->list[i];
				j++;
			}

			free(big->list);
			big->list = new_list;
			big->len_decimal = big->len_decimal - big->len_total + MAX_LEN;
			big->len_total = MAX_LEN;
			return 0;
		}
	}
	return 0;
}

void print_bigint(bigint* big)
{
	int i=(big->len_total)-1;
	int idx_dot = (big->len_decimal);
	if(big->is_neg)
	{
		printf("-");
	}
	for(;i>=idx_dot;i--)
	{
		printf("%d",big->list[i]);
	}
	if(i>=0)
	{
		printf(".");
	}
	for(;i>=0;i--)
	{
		printf("%d",big->list[i]);
	}
	printf("\n");
}

int main()
{
	char s[100];
	// scanf("%s",s);
	// bigint* big1 = conv_str_to_bigint(0,s);
	// print_bigint(big1);
	bigint* big2 = (bigint*) calloc(1,sizeof(bigint));
	big2->list = (int*) calloc(6,sizeof(int));
	int i=0;
	for(i=1;i<7;i++)
	{
		big2->list[i-1] = i;
	}
	big2->len_total = 6;
	big2->len_decimal = 3;
	big2->is_neg = 1;
	print_bigint(big2);
	reduce(big2);
	print_bigint(big2);
	// printf("%d\n",LowPrec);
	return 0;
}