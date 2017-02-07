%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "assn3.h"
%}

%type<float_val> term
%type<float_val>factor exp;
%token<float_val> NUMBER
%token ADD SUB MUL DIV SQRT LOG POW
%token OP CP
%token EOL
%union {
	int int_val;
	float float_val;
	char* str;
}

%%

calclist:
	|	calclist exp EOL { printf("Result => %f\n",$2); }
	;

exp: factor
	|	exp ADD factor { $$ = $1 + $3; printf("Adding %f + %f \n",$1,$3);}
	|	exp SUB factor { $$ = $1 - $3; }
	;

factor: term
	|	factor MUL term {$$ = $1 * $3; }
	|	factor DIV term {$$ = $1 / $3; }
	;

term: NUMBER {$$ = $1; printf(" num = %f\n",$1); } 
	| SUB NUMBER {$$ = -1*$2; }
	| OP exp CP {$$ = $2;}
	| SQRT term {$$ = 1+$2;}
	| LOG term {$$ = -1+$2;}
	| POW term {$$ = 2*$2;printf("powing - %f\n",$2);}
	;

%%

main(int argc, char* argv[])
{
	freopen("input.txt","r",stdin);
	yyparse();
}

yyerror(char *s)
{
	fprintf(stderr, "error - %s\n",s);
}