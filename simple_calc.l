%{
	#include "simple_calc.tab.h"
	#include <string.h>
	#include <stdlib.h>
%}

%%
"(" {
		return OP;
	}

")" {
		return CP;
	}

"+" {
		//printf("PLUS\n");
		return ADD;
	}
"-" {
		//printf("MINUS\n");
		return SUB;
	}
"*" {
		//printf("MULTIPLY\n");
		return MUL;
	}
"/" {
		//printf("DIVIDE\n");
		return DIV;
	}

[0-9]+ {
	//yylval = atoi(yytext);
	yylval.float_val = atof(yytext);
	printf("Number - %f %s",yylval.float_val,yytext);
	return NUMBER;
	}

"SQRT" {
			return SQRT;
		}
"POW" {
			return POW;
		}
"LOG" {
			return LOG;
		}
. {
	//printf("IGNORE\n");
	}
\n {
	return EOL;
	}

%%;