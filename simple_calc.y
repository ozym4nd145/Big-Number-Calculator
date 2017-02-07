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

%type<big_i> term
%type<big_i> factor exp;
%token<stri> NUMBER
%token ADD SUB MUL DIV SQRT LOG POW
%token OP CP
%token EOL

%%

calclist:
	|	calclist exp EOL {
							printf("Result => ");
							print_bigint($2);
						}
	;

exp: factor
	|	exp ADD factor { $$ = add($1,$3,1);}
	|	exp SUB factor { $$ = sub($1,$3,1); }
	;

factor: term
	|	factor MUL term {$$ = mult($1,$3); }
	|	factor DIV term {$$ = div_big($1,$3); }
	;

term: NUMBER {
			//printf(" num = %s\n",$1);
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
	| OP exp CP {$$ = $2;}
	| SQRT term {$$ = big_sqrt($2);}
	| LOG term {$$ = big_log($2);}
	| POW term {$$ = $2;}
	;

%%

main(int argc, char* argv[])
{
	//freopen("input.txt","r",stdin);
	scanf("%d ",&MAX_LEN);
	yyparse();
}

yyerror(char *s)
{
	fprintf(stderr, "error - %s\n",s);
}