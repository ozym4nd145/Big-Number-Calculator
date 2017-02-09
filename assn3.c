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
	while(big->list[i] == 0 && i>=big->len_decimal)
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
		error();
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
		{if(reduce(MAX_LEN,new_big) == -1)
		{
			//RAISE ERROR;
			throw_err();
		}}
	}


	free(new_a);
	free(new_b);
	return new_big;
}

bigint* digmult(bigint* a,int x)
{
 	// printf("----------------------------------------\n");
 	bigint* new_big = (bigint*) calloc(1,sizeof(bigint));
 	// print_bigint(a);
 	// printf("Mult by - %d\n",x);
 	int len_a = calc_len(a);
 	// printf("LENA = %d\narr_len - %d\n",len_a,a->arr_len);
 	// int len_a = -1*max(-1*a->arr_len,-1*alen);
 	// int len_a = a->arr_len;
 	// new_big->arr_len = a->arr_len + 1;
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
	// printf("MAX_LEN - %d\n",MAX_LEN);
	// printf("Multiplying \n");
	// print_bigint(a);
 	// printf("A: LENA = %d\narr_len - %d\n",calc_len(a),a->arr_len);
	// print_bigint(b);
 	// printf("B: LENA = %d\narr_len - %d\n",calc_len(b),b->arr_len);

 	bigint* ans = retzero();
 	int len_a = a->arr_len;
 	int true_len = calc_len(a);
 	int i = true_len-1;
 	for(i = true_len-1;i>=0;i--)
 	{
 		int x = a->list[i];
 		ans = add(digmult(b,x),digmult(ans,10),0);
 		// print_bigint(ans);
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
	while((lessthanequal(ab(div_big(sub(a,mult(ans,ans,0),1),a)),err))==0)
	{
		// printf("-----------------LOOP----------------------\n");
		// printf("Error => ");print_bigint(err);
		// bigint* debug = ab(div_big(sub(a,mult(ans,ans,0),1),a));
		// printf("DEBUG => ");print_bigint(debug);
		// printf("LOOP LESS ANS => %d\n",lessthanequal(ab(div_big(sub(a,mult(ans,ans,0),1),a)),err));
		bigint* fx = sub(mult(ans,ans,0),a,1);
		// printf("fx - ");print_bigint(fx);
		// printf("two - ");print_bigint(two);
		bigint* fdashx = mult(ans,two,0);
		// printf("fdashx - ");print_bigint(fdashx);
		bigint* temp = div_big(fx,fdashx);
		// printf("Divide - ");print_bigint(temp);
		bigint* new = sub(ans,temp,1);
		// printf("New - ");print_bigint(new);
		ans = new;
		// printf("-------------------------------------------------\n");
	} 
	reduce(MAX_LEN,ans);
	return ans;     
} 
bigint* big_log(bigint* a)
{
	bigint* one = conv_str_to_bigint(0,"1");
	if(lessthanequal(a,retzero())==1)
 	     { 
 	     	LogErr = 1;
 	     	throw_err();
 	     }
 	else if(iszero(sub(a,one,1))==1)
 	       return retzero();     
 	bigint* ans = retzero();  
 	bigint* zer = retzero();
 	bigint* err = conv_str_to_bigint(0,"1");
 	err->len_decimal =-1*max(1-MAX_LEN,-5);

 	char log_10[] = "2.3025137650431014377934029533004176534526905103187315981639\0";
    if(MAX_LEN < strlen(log_10))
    {
    	log_10[MAX_LEN] = '\0';
    }
    bigint* log10 = conv_str_to_bigint(0,log_10);

 	if(lessthan(a,one)==1)
 	{
 		bigint* term = sub(one,a,1);
 		bigint* curterm = sub(a,one,1);
 	    ans = sub(a,one,1);
 		bigint* cntr = sub(one,zer,1);
 		while(lessthanequal(ab(curterm),err)==0)
 		{
 			curterm = mult(curterm,cntr,0);
 			cntr = add(cntr,one,1);
 			curterm = div_big(curterm,cntr);
 			curterm = mult(curterm,term,0);
 			ans = add(ans,curterm,1);
 		}
 	    ans = div_big(ans,log10);
 	}
 	else
 	{
 		int no_dig = calc_len(a) - a->len_decimal ;
 		bigint* ten_pow = (bigint*) calloc(1,sizeof(bigint));
 	    ten_pow->arr_len = MAX_LEN;
 	    ten_pow->list = (int*) calloc(MAX_LEN,sizeof(int));
 	    ten_pow->len_decimal = 0;
 	    int i;
 	    for(i =0;i<no_dig;i++)
 	    	 ten_pow->list[i] = 0;
 	    ten_pow->list[i] = 1;
 	    bigint* newnum = div_big(a,ten_pow);
 	    ans = big_log(newnum);
 	    char x[MAX_LEN+2];
 	    sprintf(x,"%d",no_dig);
 	    bigint* mantissa = conv_str_to_bigint(0,x);
 	    // print_bigint(mantissa);
 	    	
 	    // mantissa = mult(mantissa,log10,0);
 	    ans = add(ans,mantissa,1);
 	}
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
	for(i=0;i<(len_new_b-len_new_a);i++)
	{
		new_a = digmult(new_a,10);
	}
	for(i=0;i<(len_new_a-len_new_b);i++)
	{
		new_b = digmult(new_b,10);
	}
	i=MAX_LEN-1; //denotes current position

	int decimal_on = 0;

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
			new_a = digmult(new_a,10);
		}
		int curr = 0;
		while(lessthanequal(new_b,new_a))
		{
			new_a = sub(new_a,new_b,0);
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

bigint* power(bigint* a, bigint* b)
{
	reduce(MAX_LEN,a);
	reduce(MAX_LEN,b);
	//check pow ( 1234.00,1.000)
	if(b->len_decimal != 0)
	{
		FracPowErr = 1;
		throw_err();
	}
	bigint* new_a = clone_big(a);
	bigint* new_b = clone_big(b);
	bigint* ans = conv_str_to_bigint(0,"1.0");
	bigint* two = conv_str_to_bigint(0,"2.0");
	if(iszero(b))
	{
		return ans;
	}
	if(b->is_neg == 1)
	{
		// NegPowErr = 1;
		// throw_err();
		new_a = div_big(ans,new_a);
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
			new_a = mult(new_a,new_a,0);
			new_b = div_big(new_b,two);
		}
		else
		{
			ans = mult(ans,new_a,0);
			new_a = mult(new_a,new_a,0);
			new_b->list[0]--;
			new_b = div_big(new_b,two);
		}
	}
	del_big(new_a);
	del_big(new_b);
	del_big(two);
	reduce(MAX_LEN,ans);
	return ans;

}

// int main()
// {
// 	char s[100];
// 	freopen("input.txt","r",stdin);
// 	MAX_LEN=20;
// 	scanf("%s",s);
// 	bigint* big1 = conv_str_to_bigint(0,s);
// 	big1->is_neg=1;
// 	// print_bigint(big1);
	
// 	scanf("%s",s);
// 	bigint* big2 = conv_str_to_bigint(0,s);
// 	// big2->is_neg = 1;
// 	// print_bigint(big2);
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
// 	bigint* div1 = div_big(big1,big2);
// 	debug_print(div1,"ANS");
// 	print_bigint(div1);
// 	// bigint* sq = big_sqrt(big1);
// 	// print_bigint(sq);
// 	// printf("Logging\n");
// 	// bigint* lg = big_log(big1);
// 	// print_bigint(lg);
// 	// printf("Powing\n");
// 	// bigint* po = power(big1,big2);
// 	// print_bigint(po);
// 	return 0;
// }
