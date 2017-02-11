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
int FracPowErr = 0;
int NegPowErr = 0;
/*
Storing with least significant digit in front of the array
*/
void throw_err()
{
	if(SynErr)
	{
		printf("SynErr\n");
	}
	if(DivErr)
	{
		printf("DivErr\n");
	}
	if(SqrtErr)
	{
		printf("SqrtErr\n");
	}
	if(LowPrec)
	{
		printf("LowPrec\n");
	}
	if(LogErr)
	{
		printf("LogErr\n");
	}
	if(FracPowErr)
	{
		printf("FracPowErr\n");
	}
	if(NegPowErr)
	{
		printf("NegPowErr\n");
	}
	exit(-1);
}

int max(int a, int b)
{
	return (a>b)?a:b;
}

int calc_len(bigint* big)
{
	int len = big->arr_len;
	// printf("Len - %d\nLen decimal - %d\n",len,big->len_decimal);
	int i=len-1;
	// print_list(big->list,len);
	// printf("Ha - %d\nI - %d\nLim - %d\n",big->list[i],i,big->len_decimal);
	while(i>=big->len_decimal && big->list[i] == 0)
	{
		len--;
		i--;
	}
	// printf("Returning - %d\n",len);
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
	// printf("MAX_LEN - %d\n",MAX_LEN);
	// printf("Converting - %s\n",string);
	int len_str = strlen(string);
	bigint* big = (bigint*) calloc(1,sizeof(bigint));
	big->list = (int*)calloc(MAX_LEN,sizeof(int));	//Initializes to zero
	big->arr_len = MAX_LEN;
	big->is_neg = is_neg;
	int i=len_str-1;
	int j=0;
	for(;i>=0;i--)
	{
		// printf("LOOPA\n");
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
	// printf("OTA\n");
	// printf("Final_len (1) => %d\n",big->arr_len);
	reduce(MAX_LEN,big);
	// printf("Final_len (2) => %d\n",big->arr_len);

	return big;
}

int reduce(int MAX_LEN,bigint* big)
{
	int decimal_length = big->len_decimal;
	int number_length = calc_len(big);
	int integer_length = number_length - decimal_length;

	if(integer_length > MAX_LEN)
	{
		LowPrec = 1;
		throw_err();
	}
	int* new_list = (int*) calloc(MAX_LEN,sizeof(int));

	int len_insignificant_zeros = 0;
	int i=0;
	while(i<decimal_length && big->list[i]==0)
	{
		len_insignificant_zeros++;
		i++;
	}

	if((number_length - len_insignificant_zeros) <= MAX_LEN)
	{
		int i=len_insignificant_zeros,j=0;
		while(i<number_length)
		{
			new_list[j]=big->list[i];
			i++;
			j++;
		}
		big->len_decimal -= len_insignificant_zeros;
	}
	else
	{
		int i=number_length-1,j=MAX_LEN-1;
		while(j>=0)
		{
			new_list[j] = big->list[i];
			j--;
			i--;
		}
		big->len_decimal = MAX_LEN - integer_length;
	}
	free(big->list);
	big->list = new_list;
	big->arr_len = MAX_LEN;
	return 0;

	// int len_total = calc_len(big);
	// int trailing_zeros = 0;
	// int i=0;
	// for(i=0;i<big->len_decimal&&big->list[i]==0;i++)
	// {
	// 	trailing_zeros++;
	// }
	// int final_len = len_total - trailing_zeros;
	// int final_decimal_len = big->len_decimal - trailing_zeros;
	// int final_integer_len = final_len - final_decimal_len;

	// int* new_list = (int *) calloc(MAX_LEN,sizeof(int));
	
	// if(final_integer_len > MAX_LEN)
	// {
	// 	LowPrec = 1;
	// 	throw_err();
	// 	return -1;
	// }

	// if(final_len <= MAX_LEN)
	// {
	// 	int j=trailing_zeros;
	// 	for(i=0;i<final_len;i++,j++)
	// 	{
	// 		new_list[i] = big->list[j];
	// 	}
	// 	big->len_decimal -= trailing_zeros;
	// }
	// else
	// {
	// 	int i= len_total - MAX_LEN;
	// 	int j=0;
	// 	for(;i<len_total;i++)
	// 	{
	// 		new_list[j] = big->list[i];
	// 		j++;
	// 	}

	// 	big->len_decimal = MAX_LEN - final_integer_len;
	// }
	// free(big->list);
	// big->list = new_list;
	// big->arr_len = MAX_LEN;
	// return 0;
}


void print_list(int* l,int len)
{
	int i=0;
	for(i=0;i<len;i++)
		printf("%d,",l[i]);
	printf("\n");
}

void debug_print(bigint* big,char* s)
{
    printf("-----------\n");
	printf("%s\nLEN_ARR => %d, len_decimal => %d\nIsNeg => %d\nList => ",s,big->arr_len,big->len_decimal,big->is_neg);
    print_list(big->list,big->arr_len);
    printf("-----------\n");
}

void print_bigint(bigint* biga)
{
	bigint* big = clone_big(biga);
	reduce(MAX_LEN/2,big);

	int len_total = calc_len(big);

	int i=(len_total)-1;
	int idx_dot = (big->len_decimal);
	if(iszero(big))
	{
		printf("0\n");
		return;
	}
	if(big->is_neg)
	{
		printf("-");
	}
	if(idx_dot >= len_total)
	{
		printf("0.");
		int i=0;
		for(i=0;i<(big->len_decimal-len_total);i++)
		{
			printf("0");
		}
		for(i=len_total-1;i>=0;i--)
		{
			printf("%d",big->list[i]);
		}
	}
	else
	{
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
	}
	printf("\n");
	del_big(big);
}

bigint* sub(bigint* a,bigint* b,int ifred)
{
	b->is_neg = (b->is_neg+1)%2;
	bigint* result= add(a,b,ifred);
	b->is_neg = (b->is_neg+1)%2;
	return result;
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
		{
			if(reduce(MAX_LEN,new_big) == -1)
			{
				//RAISE ERROR;
				throw_err();
			}
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
			int i=0;
			for(i=0;i<len;i++)
			{
				temp = (9 - new_big->list[i] + carry);	
				new_big->list[i] = temp % 10;
				carry = temp/10;
			}
			new_big->list[len] = carry;
		}
		// print_list(new_big->list,len+1);
		if(ifred)
			reduce(MAX_LEN,new_big);
	}

	free(new_a);
	free(new_b);
	return new_big;
}

bigint* digmult(bigint* a,int x)
{
 	bigint* new_big = (bigint*) calloc(1,sizeof(bigint));
 	int len_a = calc_len(a);
 	new_big->arr_len = len_a + 1;
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

bigint* mult(bigint* a,bigint* b,int ifred)
{
 	bigint* ans = retzero();
 	int len_a = a->arr_len;
 	int true_len = calc_len(a);
 	int i = true_len-1;
 	for(i = true_len-1;i>=0;i--)
 	{
 		bigint* dig_1 = digmult(b,a->list[i]);
 		bigint* dig_2 = digmult(ans,10);
 		del_big(ans);
 		ans = add(dig_1,dig_2,0);
 		del_big(dig_1);
 		del_big(dig_2);
 	}
 	ans->len_decimal = b->len_decimal + a->len_decimal;
 	ans->is_neg = (a->is_neg + b->is_neg) % 2;
 	if(ifred)
 		reduce(MAX_LEN,ans);
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
	{
		del_big(diff);
		return 0;
	}
	if(diff->is_neg == 1)
	{
		del_big(diff);
	    return 1;
	}
	del_big(diff);
	return 0;     	
}
int lessthanequal (bigint* a,bigint* b)
{
 	bigint* diff = sub(a,b,0);
 	if(iszero(diff))
 	{
 		del_big(diff);
 		return 1;
 	}
 	if(diff->is_neg == 1)
 	{
 		del_big(diff);
 	    return 1;
 	}
 	del_big(diff);
 	return 0;     	
}

int isEqual(bigint* a, bigint* b)
{
	bigint* sub_term = sub(a,b,1);
	if(iszero(sub_term))
	{
		del_big(sub_term);
		return 1;
	}
	del_big(sub_term);
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
	{
		return retzero();
	}
	if(a->is_neg == 1)
    { 
    	SqrtErr = 1;
    	throw_err();
    }
	bigint* ans = retzero();
	ans->list[1]=1;
	bigint* err = conv_str_to_bigint(0,"1");
    err->len_decimal = (MAX_LEN/2);
	bigint* two = conv_str_to_bigint(0,"2.0");

	bigint* mul_term = mult(ans,ans,0);
	bigint* sub_term = sub(a,mul_term,1);
	bigint* div_term = div_big(sub_term,a);
	bigint* abs_term = ab(div_term);

	while(lessthanequal(abs_term,err)==0)
	{
		del_big(mul_term);
		del_big(sub_term);
		del_big(div_term);
		del_big(abs_term);

		mul_term = mult(ans,ans,0);
		bigint* fx = sub(mul_term,a,1);

		del_big(mul_term);

		bigint* fdashx = mult(ans,two,0);
		bigint* temp = div_big(fx,fdashx);

		del_big(fx);
		del_big(fdashx);

		bigint* new = sub(ans,temp,1);

		del_big(ans);
		del_big(temp);

		ans = new;
		mul_term = mult(ans,ans,0);
		sub_term = sub(a,mul_term,1);
		div_term = div_big(sub_term,a);
		abs_term = ab(div_term);
		// printf("-------------------------------------------------\n");
	}
	del_big(mul_term);
	del_big(sub_term);
	del_big(div_term);
	del_big(abs_term);
	del_big(err);
	del_big(two);
	reduce(MAX_LEN,ans);
	return ans;     
}

bigint* big_log(bigint* a1)
{
	bigint* a = clone_big(a1);
	bigint* one = conv_str_to_bigint(0,"1");
 	bigint* zer = retzero();
	if(lessthanequal(a,zer)==1)
    { 
  		del_big(one);
 		del_big(zer);
	   	LogErr = 1;
    	throw_err();
    }
 	else if(isEqual(a,one)==1)
 	{
 		del_big(one);
		return zer;     
	}
 	bigint* ans = retzero();  
 	bigint* err = conv_str_to_bigint(0,"1");
 	err->len_decimal = -1*max(1-(MAX_LEN/2),-5);

 	char log_10[] = "2.3025137650431014377934029533004176534526905103187315981639\0";
    if(MAX_LEN < strlen(log_10))
    {
    	log_10[MAX_LEN] = '\0';
    }
    bigint* log10 = conv_str_to_bigint(0,log_10);

    bigint* temp;

 	if(lessthan(a,one)==1)
 	{
 		int startzero = 0;
 		for(int ii = (a->len_decimal-1);ii>=0;ii--)
 		{
 			if(a->list[ii]!=0)
 				break;
 			startzero++;
 		}
 		a->len_decimal = a->len_decimal - startzero;
 		bigint* term = sub(one,a,1);
 		bigint* curterm = sub(a,one,1);
 		del_big(ans);
 	    ans = sub(a,one,1);
 		bigint* cntr = clone_big(one);
 		temp = ab(curterm);
 		while(lessthanequal(temp,err)==0)
 		{
 			del_big(temp);
 			temp = mult(curterm,cntr,0);
 			del_big(curterm);
 			curterm = temp;

 			temp = add(cntr,one,1);
 			del_big(cntr);
 			cntr = temp;

 			temp = div_big(curterm,cntr);
 			del_big(curterm);
 			curterm = temp;

 			temp = mult(curterm,term,0);
 			del_big(curterm);
 			curterm = temp;

 			temp = add(ans,curterm,1);
 			del_big(ans);
 			ans = temp;
 			temp = ab(curterm);
 		}
 		del_big(cntr);
 	    del_big(term);
 	    del_big(curterm);
 	    del_big(temp);
 	    temp = div_big(ans,log10);
 	    del_big(log10);
 	    del_big(err);
 	    del_big(one);
 	    del_big(zer);
 	    del_big(ans);
 	    char x[MAX_LEN+2];
 	    sprintf(x,"%d",startzero);
 	    bigint* mantissa = conv_str_to_bigint(1,x);
 	    if(iszero(mantissa)==0)
 	       temp = add(temp,mantissa,1);
 	    ans = temp;
 	}
 	else
 	{
 		del_big(log10);
 	    del_big(err);
 	    del_big(one);
 	    del_big(zer);
 	    del_big(ans);
 		int no_dig = calc_len(a) - a->len_decimal ;
 		int orig_dec_len = a->len_decimal;
 		a->len_decimal = no_dig + orig_dec_len;
 		// bigint* ten_pow = (bigint*) calloc(1,sizeof(bigint));
 	 //    ten_pow->arr_len = MAX_LEN;
 	 //    ten_pow->list = (int*) calloc(MAX_LEN,sizeof(int));
 	 //    ten_pow->len_decimal = 0;
 	 //    int i;
 	 //    for(i =0;i<no_dig;i++)
 	 //    	 ten_pow->list[i] = 0;
 	 //    ten_pow->list[i] = 1;
 	 //    bigint* newnum = div_big(a,ten_pow);
 	 //    ans = big_log(newnum);
 	    ans = big_log(a);

 	    a->len_decimal = orig_dec_len;
 	    
 	    char x[MAX_LEN+2];
 	    sprintf(x,"%d",no_dig);
 	    bigint* mantissa = conv_str_to_bigint(0,x);
 	    // print_bigint(mantissa);
 	    	
 	    // mantissa = mult(mantissa,log10,0);
 	    temp = add(ans,mantissa,1);
 	    del_big(mantissa);
 	    del_big(ans);
 	    ans = temp;
 	}
 	del_big(a);
 	return ans;  
}

bigint* clone_big(bigint* a)
{
	bigint* new_big = (bigint*)calloc(1,sizeof(bigint));
	new_big->list = (int*)calloc(a->arr_len,sizeof(int));
	new_big->arr_len = a->arr_len;
	new_big->is_neg = a->is_neg;
	new_big->len_decimal = a->len_decimal;
	int i=0;
	for(i=0;i<a->arr_len;i++)
	{
		new_big->list[i] = a->list[i];
	}
	return new_big;
}

void del_big(bigint* big)
{
	free(big->list);
	free(big);
}

bigint* div_big(bigint* a,bigint* b)
{
	/**
	 * A is divided by B
	 */
	if(iszero(b))
	{
		DivErr = 1;
		throw_err();
	}	
	bigint* new_a = clone_big(a);
	bigint* new_b = clone_big(b);
	reduce(MAX_LEN,new_a);
	reduce(MAX_LEN,new_b);

	bigint* quo = (bigint*) calloc(1, sizeof(bigint));
	quo->list = (int*) calloc(MAX_LEN,sizeof(int));
	quo->arr_len = MAX_LEN;
	quo->is_neg = (new_a->is_neg + new_b->is_neg)%2;

	new_a->is_neg = 0;
	new_b->is_neg = 0;

	int decimal_len = new_a->len_decimal - new_b->len_decimal;
	new_a->len_decimal=0;
	new_b->len_decimal=0;
	int len_new_a = calc_len(new_a);
	int len_new_b = calc_len(new_b);
	decimal_len += len_new_b - len_new_a;

	int i=0;

	/**
	 * Made a and b of same length
	 */
	if((len_new_b - len_new_a) > 0)
	{
		int i=len_new_a-1,j=(len_new_b - len_new_a);
		while(i>=0)
		{
			new_a->list[i+j] = new_a->list[i];
			i--;
		}
		j--;
		while(j>=0)
		{
			new_a->list[j] = 0;
			j--;
		}
	}
	else if ((len_new_b - len_new_a) < 0)
	{
		int i=len_new_b-1,j=(len_new_a - len_new_b);
		while(i>=0)
		{
			new_b->list[i+j] = new_b->list[i];
			i--;
		}
		j--;
		while(j>=0)
		{
			new_b->list[j] = 0;
			j--;
		}
	}
	// for(i=0;i<(len_new_b-len_new_a);i++)
	// {
	// 	new_a = digmult(new_a,10);
	// }
	// for(i=0;i<(len_new_a-len_new_b);i++)
	// {
	// 	new_b = digmult(new_b,10);
	// }
	i=MAX_LEN-1; //denotes current position

	int decimal_on = 0;
	bigint* temp;

	for(;i>=0;i--)
	{
		// printf("------------------------------------------------------\n");
		// printf("Iteration - %d\nDecimal_On - %d\n",MAX_LEN - i,decimal_on);
		// printf("Dividend - ");print_bigint(new_b);
		// printf("Divisor - ");print_bigint(new_a);
		// printf("Quotient - ");print_bigint(quo);
		// printf("Decimal_len - %d\n",decimal_len);
		// printf("------------------------------------------------------\n");
		if(!decimal_on && lessthan(new_a,new_b))
		{
			decimal_on = 1;
		}
		if(decimal_on)
		{
			temp = digmult(new_a,10);
			del_big(new_a);
			new_a = temp;
		}
		int curr = 0;
		while(lessthanequal(new_b,new_a))
		{
			temp = sub(new_a,new_b,0);
			del_big(new_a);
			new_a = temp;
			curr++;
		}
		quo->list[i] = curr;
		if(decimal_on)
		{
			decimal_len++;
		}
	}
	if(decimal_len >= 2*MAX_LEN)
	{
		free(quo->list);
		quo->list = (int*) calloc(MAX_LEN,sizeof(int));
	}
	else if(decimal_len > MAX_LEN)
	{
		int i=0;
		int j=decimal_len-MAX_LEN;
		for(;j<MAX_LEN;j++,i++)
		{
			quo->list[i] = quo->list[j];
		}
		for(;i<MAX_LEN;i++)
		{
			quo->list[i] = 0;
		}
		quo->len_decimal = MAX_LEN;
	}
	else
	{
		quo->len_decimal = decimal_len;
	}
	del_big(new_b);
	del_big(new_a);
	// print_list(quo->list,MAX_LEN);
	reduce(MAX_LEN,quo);
	// print_list(quo->list,MAX_LEN);
	
	// printf("quo\nLEN_ARR => %d, len_decimal => %d\nList => ",quo->arr_len,quo->len_decimal);
 //    print_list(quo->list,MAX_LEN);
 //    printf("NUMBER => \n");print_bigint(quo);

	return quo;
}

bigint* powerint(bigint* a, bigint* b)
{
	bigint* new_a = clone_big(a);
	bigint* new_b = clone_big(b);
	bigint* temp;
	reduce(MAX_LEN,new_a);
	reduce(MAX_LEN,new_b);
	//check pow ( 1234.00,1.000)
	if(new_b->len_decimal != 0)
	{
		FracPowErr = 1;
		del_big(new_a);
		del_big(new_b);
		throw_err();
	}
	bigint* ans = conv_str_to_bigint(0,"1.0");
	bigint* two = conv_str_to_bigint(0,"2.0");
	if(iszero(b))
	{
		del_big(new_a);
		del_big(new_b);
		del_big(two);
		return ans;
	}
	if(b->is_neg == 1)
	{
		// NegPowErr = 1;
		// throw_err();
		temp = div_big(ans,new_a);
		//del_big(temp);
		new_a = temp;
		new_b->is_neg = 0;
		
	}

	// print_bigint(new_a);
	// printf("Ki power ");
	// print_bigint(new_b);

	while(!iszero(new_b))
	{
		// printf("Power - ");print_bigint(new_b);
		// printf("Ans - ");print_bigint(ans);
		if(new_b->list[0]%2 == 0)
		{
			temp = mult(new_a,new_a,0);
			del_big(new_a);
			new_a = temp;

			temp = div_big(new_b,two);
			del_big(new_b);
			new_b = temp;
		}
		else
		{
			temp = mult(ans,new_a,0);
			del_big(ans);
			ans = temp;

			temp = mult(new_a,new_a,0);
			del_big(new_a);
			new_a = temp;

			new_b->list[0]--;

			temp = div_big(new_b,two);
			del_big(new_b);
			new_b = temp;
			
		}
	}
	del_big(new_a);
	del_big(new_b);
	del_big(two);
	reduce(MAX_LEN,ans);
	return ans;

}
bigint* integralpart(bigint* a)
 {
 	bigint* ans = clone_big(a);
 	int l1 = calc_len(a) - a->len_decimal;
 	reduce(l1,ans);
 	return ans;
 }
 bigint* fractionalpart(bigint* a)
 {
 	bigint* ans = integralpart(a);
 	ans = sub(a,ans,1);
 	return ans;
 }
 bigint* power(bigint* a, bigint* b)
{
	bigint* new_a = clone_big(a);
	bigint* new_b = clone_big(b);
	bigint* temp;
	reduce(MAX_LEN,new_a);
	reduce(MAX_LEN,new_b);
	//check pow ( 1234.00,1.000)
	
	bigint* ans = conv_str_to_bigint(0,"1.0");
	bigint* two = conv_str_to_bigint(0,"2.0");
	if(iszero(b))
	{
		del_big(new_a);
		del_big(new_b);
		del_big(two);
		return ans;
	}
	if(b->is_neg == 1)
	{
		// NegPowErr = 1;
		// throw_err();
		temp = div_big(ans,new_a);
		new_a = temp;
		//del_big(temp);
		new_b->is_neg = 0;
		
	}
	bigint* intb = integralpart(new_b);
	bigint* fracb = fractionalpart(new_b);
	bigint* intpow = powerint(new_a,intb);
	bigint* fracpow = powerfrac(new_a,fracb);
	if(iszero(fracb))
		ans = clone_big(intpow);
	else
	    ans = mult(intpow,fracpow,1);
	del_big(new_a);
	del_big(new_b);
	del_big(two);
	del_big(intb);
	del_big(fracb);
	del_big(intpow);
	del_big(fracpow);
	reduce(MAX_LEN,ans);
	return ans;

}
bigint* ex(bigint* a)   // e^a
{
	bigint* ans = retzero();
	bigint* term = conv_str_to_bigint(0,"1");
	bigint* cntr = conv_str_to_bigint(0,"1");
	bigint* one = conv_str_to_bigint(0,"1");
	bigint* err = conv_str_to_bigint(0,"1");
 	err->len_decimal = -1*max(1-(MAX_LEN/2),-5);
 	while(lessthanequal(term,err)==0)
 	{
 		ans = add(ans,term,0);
 		term = mult(term,a,0);
 		term = div_big(term,cntr);
 		cntr = add(cntr,one,0);
 	}
 	reduce(MAX_LEN,ans);
 	del_big(term);
 	del_big(cntr);
 	del_big(one);
 	del_big(err);
 	return ans;
}
bigint* powerfrac(bigint* a,bigint* b) // a^b = e^(b*ln(a))
 {
 	//printf("entering power frac \n");
 	bigint* lna = big_log(a);
 	char log_10[] = "2.3025137650431014377934029533004176534526905103187315981639\0";
    if(MAX_LEN < strlen(log_10))
    {
    	log_10[MAX_LEN] = '\0';
    }
    bigint* log10 = conv_str_to_bigint(0,log_10);
    lna = mult(lna,log10,0);
 	//printf(" ln a : \n");
 	//print_bigint(lna);
 	bigint* blna = mult(b,lna,0);
 	//printf(" b* ln a : \n");
 	//print_bigint(blna);
 	bigint* ans = ex(blna);
 	del_big(lna);
 	del_big(blna);
 	return ans;
 }
// int main()
// {
//	char s[100];
// 	// freopen("input1.txt","r",stdin);
// 	MAX_LEN=6000;
//  	scanf("%s",s);
// 	bigint* big1 = conv_str_to_bigint(0,s);	
// 	//big1->is_neg=1;
// 	print_bigint(big1);
// 	bigint* expo = ex(big1);
// 	printf("e raised to power : ");
// 	print_bigint(expo);
//	bigint* integ = integralpart(big1);
//	bigint* frac =  fractionalpart(big1);
//	printf("integral part ");
//	print_bigint(integ);
//	printf("fractional part ");
//	print_bigint(frac);
// scanf("%s",s);
// bigint* big2 = conv_str_to_bigint(0,s);
// 	// big2->is_neg = 1;
// print_bigint(big2);
// printf("a raised to power b :");
// bigint* apowb = powerfrac(big1,big2);
// print_bigint(apowb);
// 	// bigint* subbed = sub(big1,big2,1);
// 	// print_bigint(subbed);
// 	// print_bigint(big1);
// 	// print_bigint(big2);
// 	// bigint* mul = mult(big1,big2);
// 	// print_bigint(mul);
// 	// print_bigint(big1);
// 	// print_bigint(big2);
// 	// printf("less than equal %d\n",lessthanequal(big1,big2));
// 	// print_bigint(big1);
// 	// print_bigint(big2);
// 	//bigint* div1 = div_big(big1,big2);
// 	//debug_print(div1,"ANS");
// 	//print_bigint(div1);
// 	// bigint* sq = big_sqrt(big1);
// 	// print_bigint(sq);
// 	// printf("Logging\n");
// 	// bigint* lg = big_log(big1);
// 	// print_bigint(lg);
// 	// printf("Powing\n");
// 	// bigint* po = power(big1,big2);
// 	// print_bigint(po);
	
// 	printf("Factorialing\n");
// 	bigint* one = conv_str_to_bigint(0,"1");
// 	bigint* ans = clone_big(one);
// 	bigint* temp;
// 	while(!isEqual(one,big1))
// 	{
// 		print_bigint(big1);
// 		// print_bigint(ans);
// 		printf("---------------------------\n");
// 		temp = mult(ans,big1,0);
// 		del_big(ans);
// 		ans = temp;

// 		temp = sub(big1,one,0);
// 		del_big(big1);
// 		big1 = temp;
// 	}
// 	del_big(big1);
// 	print_bigint(ans);
// 	del_big(ans);
// 	return 0;
// }
