bignumcalc: bignumcalc.l bignumcalc.y bignumcalc_prog.c
	bison -d bignumcalc.y
	flex bignumcalc.l
	gcc -g -o $@ bignumcalc.tab.c lex.yy.c bignumcalc_prog.c -lfl
clean:
	rm bignumcalc.tab.*
	rm lex.yy.c
