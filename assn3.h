#ifndef ASSN3_H
#define ASSN3_H
	typedef struct
	{
		int* list;
		int len_decimal;
		int arr_len;
		int is_neg;
	} bigint;

	bigint* conv_str_to_bigint(int is_neg,char* string);
	int reduce(bigint* big);
	void print_bigint(bigint* big);
	bigint* add(bigint* a, bigint* b,int ifred);
	bigint* sub(bigint* a,bigint* b,int ifred);
	bigint* retzero();
	bigint* digmult(bigint* a,int x);
	bigint* mult(bigint* a,bigint* b);
	bigint* div_big(bigint* a,bigint* b);
	int lessthanequal (bigint* a,bigint* b);
	int lessthan (bigint* a,bigint* b);
	int iszero (bigint* a);
	bigint* clone_big(bigint* a);
	bigint* big_sqrt(bigint* a);
	bigint* big_log(bigint* a);
	bigint* ab(bigint* a);
	int iszero (bigint* a);

	extern int MAX_LEN;
#endif