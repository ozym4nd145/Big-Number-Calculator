simple_calc: simple_calc.l simple_calc.y assn3.c
	bison -d simple_calc.y
	flex simple_calc.l
	gcc -g -o $@ simple_calc.tab.c lex.yy.c assn3.c -lfl
