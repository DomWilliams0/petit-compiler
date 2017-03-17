%token CONSTANT IDENTIFIER

%{
extern int yylex();
extern int yyparse();
int yyerror(const char *p) {}
%}

%%
test: CONSTANT
%%
int main(int argc, char **argv)
{
	yyparse();
}
