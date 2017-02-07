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
	bigint* add(bigint* a, bigint* b);
	bigint* sub(bigint* a,bigint* b);
#endif