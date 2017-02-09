%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "assn3.h"
	int MAX_LEN;
%}

%union {
	int int_val;
	float float_val;
	char* stri;
	bigint* big_i;
}

%type<big_i> unit
%type<big_i> mult_div expression;
%token<stri> NUMBER
%token ADD SUB MUL DIV SQRT LOG POW COMMA
%token OP CP
%token EOL

%%

calcexpression:
	|	calcexpression expression EOL {
							printf("Result => ");
							print_bigint($2);
						}
	;

expression: mult_div
	|	expression ADD mult_div { $$ = add($1,$3,1);}
	|	expression SUB mult_div { $$ = sub($1,$3,1); }
	;

mult_div: unit
	|	mult_div MUL unit {$$ = mult($1,$3,1); }
	|	mult_div DIV unit {$$ = div_big($1,$3); }
	;

unit: NUMBER {
			bigint* big = conv_str_to_bigint(0,$1);
			//print_bigint(big);
			$$ = big;
		}

	| SUB NUMBER {
			//printf(" num = %s\n",$2);
			bigint* big = conv_str_to_bigint(1,$2);
			//print_bigint(big);
			$$ = big;
		}
	| OP expression CP {$$ = $2;}
	| SQRT unit {$$ = big_sqrt($2);}
	| LOG unit {$$ = big_log($2);}
	| POW OP unit COMMA unit CP {$$ = power($3,$5);}
	;

%%

main(int argc, char* argv[])
{
	//freopen("input.txt","r",stdin);
	scanf("%d ",&MAX_LEN);
	MAX_LEN = 2*MAX_LEN;
	yyparse();
}

yyerror(char *s)
{
	fprintf(stderr, "error - %s\n",s);
}