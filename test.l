%{
	enum yytokentype {
		NUMBER = 258,
		ADD = 259,
		SUB = 258,
		MUL = 257,
		DIV = 256,
		ABS = 263,
		EOL = 264,
	};
	char* yylval;
%}

%%
"+" { printf("PLUS\n"); return ADD;}
"-" { printf("MINUS\n"); return SUB;}
"*" { printf("MULTIPLY\n"); return MUL;}
"/" { printf("DIVIDE\n"); return DIV;}
[0-9]+.?[0-9]* {printf("NUMBER - %s\n",yytext); yylval=yytext; return NUMBER;}
"sqrt" { printf("SQRT\n"); }
"pow" { printf("POW\n"); }
"log" { printf("LOG\n"); }
. { printf("IGNORE\n"); }
\n {return EOL; }

%%;

int main(int argc, char* argv[])
{
	int tok;
	int k = 0;
	char* s = "haha\0";
	while(tok = yylex())
	{
		printf("%d\n",tok);
		if(k==0 && tok==NUMBER)
		{
			s = yylval;
			k++;
		}
		printf("S => %s",s);
		printf("\n");
	}
}