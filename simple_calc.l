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
.	{
	//printf("IGNORE\n");
	}
\n {
	return EOL;
	}

%%;