%token CONSTANT IDENTIFIER
%token IF ELSE WHILE FOR
%token RETURN TYPE
%token INC_OP DEC_OP LE_OP GE_OP EQ_OP NE_OP AND_OP OR_OP ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN

%{
#include <iostream>

extern int yylex();
extern int yyparse();
int yyerror(const char *p) { std::cout << "Error: " << p << std::endl;}
%}

%%
test: CONSTANT
%%
int main(int argc, char **argv)
{
yylex();
}
