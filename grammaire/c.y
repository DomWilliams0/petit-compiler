%token CONSTANT IDENTIFIER
%token IF ELSE WHILE FOR
%token RETURN TYPE
%token INC_OP DEC_OP LE_OP GE_OP EQ_OP NE_OP AND_OP OR_OP ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN

%left '+'
%nonassoc POS

%left '-'
%nonassoc NEG

%define parse.error verbose

%{
#include <iostream>

extern int yylex();
extern int yyparse();
int yyerror(const char *p)
{
	std::cout << "Error: " << p << std::endl;
}
%}

%start document

%%
document
	: any_decl
	| func_def
	| document any_decl
	| document func_def

var
	: IDENTIFIER

fonction_appel
	: IDENTIFIER '(' expr ')'
	| IDENTIFIER '(' ')'

lvalue
	: var
	| var '[' expr ']'

expr_uniq
	: CONSTANT
	| lvalue
	| fonction_appel '[' expr ']'
	| lvalue INC_OP
	| lvalue DEC_OP
	| INC_OP lvalue
	| DEC_OP lvalue
	| '(' expr_uniq ')'
	| '+' expr_uniq %prec POS
	| '-' expr_uniq %prec NEG
	| expr_uniq '/' expr_uniq
	| expr_uniq '*' expr_uniq
	| expr_uniq '%' expr_uniq
	| expr_uniq '+' expr_uniq
	| expr_uniq '-' expr_uniq
	| expr_uniq '<' expr_uniq
	| expr_uniq LE_OP expr_uniq
	| expr_uniq '>' expr_uniq
	| expr_uniq GE_OP expr_uniq
	| expr_uniq EQ_OP expr_uniq
	| expr_uniq NE_OP expr_uniq
	| expr_uniq AND_OP expr_uniq
	| expr_uniq OR_OP expr_uniq
	| lvalue '=' expr_uniq
	| lvalue ADD_ASSIGN expr_uniq
	| lvalue SUB_ASSIGN expr_uniq
	| lvalue MUL_ASSIGN expr_uniq
	| lvalue DIV_ASSIGN expr_uniq
	| lvalue MOD_ASSIGN expr_uniq
	| fonction_appel

expr
	: expr_uniq
	| expr_uniq ',' expr

decl_var
	: IDENTIFIER
	| IDENTIFIER '[' CONSTANT ']'  // constant expression ?
	| IDENTIFIER '['  ']'

decl
	: TYPE decl_1 ';'

decl_1
	: decl_2
	| decl_1 ',' decl_2

decl_2
	: decl_var
	| decl_var '=' expr_uniq

decl_uniq
	: TYPE decl_var

decl_args_list
	: TYPE
	| decl_uniq
	| decl_args_list ',' TYPE
	| decl_args_list ',' decl_uniq

decl_args
	: '('')'
	| '(' decl_args_list ')'

func_decl
	: TYPE IDENTIFIER decl_args ';'

any_decl
	: decl
	| func_decl

func_def
	: TYPE IDENTIFIER decl_args block

block_item
	: decl
	| stat

block_item_list
	: block_item
	| block_item_list block_item

block
	: '{' '}'
	| '{' block_item_list '}'

if_decl
	: IF '(' expr ')' stat

else
	: /* vide */
	| ELSE stat

if
	: if_decl else

expr_for
	: expr
	| /* vide */

iter
	: WHILE '(' expr ')' stat
	| FOR '(' expr_for ';' expr_for ';' expr_for ')' stat

return_stat
	: RETURN ';'
	| RETURN expr ';'

stat
	: if
	| iter
	| block
	| expr ';'
	| ';'
	| return_stat


%%
extern char *yytext;
int main(int argc, char **argv)
{
	yyparse();
}
