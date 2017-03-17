

.PHONY: default
default:
	bison -vd c.y
	flex c.l
	g++ -g -o c c.tab.c lex.yy.c

.PHONY: clean
clean:
	@rm -f *.o lex.yy.c c.tab.c c.tab.h c
