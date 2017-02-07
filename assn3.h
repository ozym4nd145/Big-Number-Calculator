typedef struct
{
	int* list;
	int len_decimal;
	int len_total;
	int is_neg;
} bigint;
bigint* conv_str_to_bigint(int is_neg,char* string);
int reduce(bigint* big);
void print_bigint(bigint* big);
bigint* add(bigint* a, bigint* b);