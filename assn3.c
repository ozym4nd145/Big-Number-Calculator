#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int MAX_LEN = 100;
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
	int arr_len;
	int is_neg;
} bigint;


void error()
{
	printf("Some error Occured");
}

int max(int a, int b)
{
	return (a>b)?a:b;
}

int calc_len(bigint* big)
{
	int len = big->arr_len;
	int i=len-1;
	while(big->list[i] == 0)
	{
		len--;
		i--;
	}
	return len;
}

bigint* conv_str_to_bigint(int is_neg,char* string)
{
	int len_str = strlen(string);
	bigint* big = (bigint*) calloc(1,sizeof(bigint));
	// big->len_total = len_str;
	big->list = (int*)calloc(MAX_LEN,sizeof(int));	//Initializes to zero
	big->arr_len = MAX_LEN;
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
			// big->len_total--;
		}
	}
	return big;
}

int reduce(bigint* big)
{
	if(big->arr_len > MAX_LEN)
	{
		int len_total = calc_len(big);
		int trailing_zeros = 0;
		int i=0;
		for(i=0;i<big->len_decimal&&big->list[i]==0;i++)
		{
			trailing_zeros++;
		}
		int final_len = len_total - trailing_zeros;
		int final_decimal_len = big->len_decimal - trailing_zeros;
		int final_integer_len = final_len - final_decimal_len;

		int* new_list = (int *) calloc(MAX_LEN,sizeof(int));
		
		if(final_integer_len > MAX_LEN)
		{
			LowPrec = 1;
			return -1;
		}

		if(final_len <= MAX_LEN)
		{
			int j=trailing_zeros;
			for(i=0;i<final_len;i++,j++)
			{
				new_list[i] = big->list[j];
			}
			big->len_decimal -= trailing_zeros;
		}
		else
		{
			//TODO: Round Off Procedure
			int i= len_total - MAX_LEN;
			int j=0;
			for(;i<len_total;i++)
			{
				new_list[j] = big->list[i];
				j++;
			}
			big->len_decimal = MAX_LEN - final_integer_len;
		}
		free(big->list);
		big->list = new_list;
		big->arr_len = MAX_LEN;
		return 0;
	}
	return 0;
}

void print_bigint(bigint* big)
{
	int len_total = calc_len(big);
	int i=(len_total)-1;
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

void print_list(int* l,int len)
{
	int i=0;
	for(i=0;i<len;i++)
		printf("%d",l[i]);
	printf("\n");
}

bigint* add(bigint* a, bigint* b)
{
	bigint* new_big = (bigint*) calloc(1,sizeof(bigint));

	/**
	 * Making the int list of both to be of same size;
	 */
	int len_a = calc_len(a);
	int len_b = calc_len(b);
	int len = max(max(a->len_decimal,b->len_decimal)+max(len_a-(a->len_decimal),len_b-(b->len_decimal)),MAX_LEN);
	int* new_a = (int*) calloc(len,sizeof(len));
	int* new_b = (int*) calloc(len,sizeof(len));
	int i=0;
	int j=0;
	for(i=(max(a->len_decimal,b->len_decimal) - a->len_decimal),j=0;j<len_a;j++,i++)
	{
		new_a[i] = a->list[j];
	}
	// print_list(new_a,len);
	for(i=(max(a->len_decimal,b->len_decimal) - b->len_decimal),j=0;j<len_b;j++,i++)
	{
		new_b[i] = b->list[j];
	}
	// print_list(new_b,len);

	new_big->list = (int*) calloc(len+1,sizeof(int));
	new_big->arr_len = len+1;	
	new_big->len_decimal = max(a->len_decimal,b->len_decimal);
	new_big->is_neg = a->is_neg;

	
	if(a->is_neg == b->is_neg)
	{
		int carry = 0;
		int temp = 0;
		for(i=0;i<len;i++)
		{
			temp = new_a[i] + new_b[i] + carry;
			carry = temp/10;
			new_big->list[i] = temp%10;
		}
		if(carry)
		{
			new_big->list[len] = carry;
		}
		// print_list(new_big->list,len+1);	
		if(reduce(new_big) == -1)
		{
			//RAISE ERROR;
			error();
		}
		// print_list(new_big->list,MAX_LEN);
	}
	else
	{
		/**
		 * Doing a - b;
		 */
		int carry = 0;
		// print_list(new_a,len);
		// print_list(new_b,len);
		for(i=0;i<len;i++)
		{
			new_big->list[i] = (new_a[i] - new_b[i] + 10 - carry)%10;
			carry = 1 - (new_a[i] - new_b[i] + 10 - carry)/10;
		}
		// print_list(new_big->list,len+1);

		/**
		 * checking if negative result
		 */
		if(carry != 0)
		{
			new_big->is_neg = (new_big->is_neg+1)%2;	//inverts the sign
			carry = 1;
			int temp = 0;
			for(int i=0;i<len;i++)
			{
				temp = (9 - new_big->list[i] + carry);	
				new_big->list[i] = temp % 10;
				carry = temp/10;
			}
			new_big->list[len] = carry;
		}
		// print_list(new_big->list,len+1);
		if(reduce(new_big) == -1)
		{
			//RAISE ERROR;
			error();
		}
	}


	free(new_a);
	free(new_b);
	return new_big;
}

int main()
{
	char s[100];
	scanf("%s",s);
	bigint* big1 = conv_str_to_bigint(0,s);
	print_bigint(big1);
	
	scanf("%s",s);
	bigint* big2 = conv_str_to_bigint(0,s);
	big2->is_neg = 1;
	print_bigint(big2);
	bigint* added = add(big1,big2);
	print_bigint(added);
	

	return 0;
}