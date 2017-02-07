#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "assn3.h"

int SynErr = 0;
int DivErr = 0;
int SqrtErr = 0;
int LowPrec = 0;
int MAX_LEN = 100;
int LogErr = 0;

/*
Storing with least significant digit in front of the array
*/
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
bigint* retzero()
 {
  bigint* big = (bigint*) calloc(1,sizeof(bigint));
  big->list = (int*)calloc(2,sizeof(int));
  big->list[0]=big->list[1]=0;
  big->is_neg = 0; //check sign
  big->arr_len = 2;
  big->len_decimal = 1;
  return big;
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

bigint* sub(bigint* a,bigint* b,int ifred)
{
	b->is_neg = (b->is_neg+1)%2;
	return add(a,b,ifred);
}


bigint* add(bigint* a, bigint* b,int ifred)
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
		if(ifred)
		{if(reduce(new_big) == -1)
		{
			//RAISE ERROR;
			error();
		}}
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
		if(ifred)
		{if(reduce(new_big) == -1)
		{
			//RAISE ERROR;
			error();
		}}
	}


	free(new_a);
	free(new_b);
	return new_big;
}

bigint* digmult(bigint* a,int x)
 {
 	bigint* new_big = (bigint*) calloc(1,sizeof(bigint));
 	int len_a = a->arr_len;
 	new_big->arr_len = a->arr_len + 1;
 	int carry = 0,i;
 	new_big->list = (int*) calloc(len_a+1,sizeof(int));
 	for(i=0;i<len_a;i++)
 	{
 		new_big->list[i] = (x*a->list[i] + carry)%10;
 		carry = (x*a->list[i] + carry)/10;
 	}
 	new_big->list[i] = carry;
 	new_big->is_neg = a->is_neg;
 	return new_big;
 }

bigint* mult(bigint* a,bigint* b)
{
 	bigint* ans = retzero();
 	int len_a = a->arr_len;
 	int true_len = calc_len(a);
 	for(int i = true_len-1;i>=0;i--)
 	{
 		int x = a->list[i];
 		ans = add(digmult(b,x),digmult(ans,10),0);
 		// print_bigint(ans);
 	}
 	ans->len_decimal = b->len_decimal + a->len_decimal;
 	if(reduce(ans)==-1)
 		 error();
 	return ans;
 } 
int iszero (bigint* a)
 {
 	int l = a->arr_len;
 	int i = 0;
 	for(i=0;i<l;i++)
 	{
 		if(a->list[i]!=0)
 			 return 0;
 	}
 	return 1;
 }
 int lessthan (bigint* a,bigint* b)
 {
 	bigint* diff = sub(a,b,0);
 	if(iszero(diff))
 		 return 0;
 	if(diff->is_neg == 1)
 	     return 1;
 	return 0;     	
 }
  int lessthanequal (bigint* a,bigint* b)
 {
 	bigint* diff = sub(a,b,0);
 	if(iszero(diff))
 		 return 1;
 	if(diff->is_neg == 1)
 	     return 1;
 	return 0;     	
 }
bigint* ab(bigint* a)
 {
 	bigint* a1 = clone_big(a);
 	a1->is_neg = 0;
 	return a1;
 }
bigint* big_sqrt(bigint* a)
 {
 	if(iszero(a))
 		 return retzero();
 	if(a->is_neg == 1)
 	     { 
 	     	SqrtErr = 1;
 	     	error();
 	     }
 	bigint* ans = retzero();
 	ans->list[1]=1;
 	bigint* err = conv_str_to_bigint(0,"0.001");
 	while(lessthanequal(ab(div_big(sub(a,mult(ans,ans),1),a)),err)==0)
 	  {
 	  	bigint* fx = sub(mult(ans,ans),a,1);
 	  	bigint* two = conv_str_to_bigint(0,"2.0");
 	  	bigint* fdashx = mult(ans,two);
 	  	bigint* new = sub(ans,div_big(fx,fdashx),1);
 	  	ans = new;
 	  } 
 	return ans;     
 } 
bigint* big_log(bigint* a)
{
	bigint* one = conv_str_to_bigint(0,"1");
	if(lessthanequal(a,retzero())==1)
 	     { 
 	     	LogErr = 1;
 	     	error();
 	     }
 	else if(iszero(sub(a,one,1))==1)
 	       return retzero();     
 	bigint* ans = retzero();  
 	bigint* zer = retzero();
 	bigint* err = conv_str_to_bigint(0,"1");
 	err->len_decimal = MAX_LEN - 2;
 	if(lessthan(a,one)==1)
 	{
 		bigint* term = sub(one,a,1);
 		bigint* curterm = sub(a,one,1);
 	    ans = sub(a,one,1);
 		bigint* cntr = sub(one,zer,1);
 		while(lessthanequal(ab(curterm),err)==0)
 		{
 			curterm = mult(curterm,cntr);
 			cntr = add(cntr,one,1);
 			curterm = div_big(curterm,cntr);
 			curterm = mult(curterm,term);
 			ans = add(ans,curterm,1);
 		}

 	}
 	else
 	{
 		bigint* term = sub(a,one,1);
 		term = div_big(term,a);
 		bigint* curterm = sub(term,zer,1);
 	    ans = sub(term,zer,1);
 		bigint* cntr = sub(one,zer,1);
 		while(lessthanequal(ab(curterm),err)==0)
 		{
 			curterm = mult(curterm,cntr);
 			cntr = add(cntr,one,1);
 			curterm = div_big(curterm,cntr);
 			curterm = mult(curterm,term);
 			ans = add(ans,curterm,1);
 		}
 	}
 	return ans;  
}
int main()
{
	char s[100];
	scanf("%s",s);
	bigint* big1 = conv_str_to_bigint(0,s);
	print_bigint(big1);
	
	scanf("%s",s);
	bigint* big2 = conv_str_to_bigint(0,s);
	big2->is_neg = 0;
	print_bigint(big2);
	bigint* added = add(big1,big2,1);
	print_bigint(added);
	bigint* mul = mult(big1,big2);
	print_bigint(mul);
	printf(" less than equal %d",lessthanequal(big1,big2));
	return 0;
}