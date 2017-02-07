%{
	#include "assn3.h"
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
		return ADD;
	}
"-" {
		return SUB;
	}
"*" {
		return MUL;
	}
"/" {
		return DIV;
	}

[0-9]+(\.[0-9]+)? {
	char* numb = strdup(yytext);	
	yylval.stri = numb;
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
[ \t\r]+ {}
\n 	{
		return EOL;
	}
.	{
		yyerror("SynErr");
	}

%%;