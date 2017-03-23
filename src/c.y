%token CONSTANT_INTEGER CONSTANT_CHARACTER IDENTIFIER
%token IF ELSE WHILE FOR
%token RETURN TYPE
%token INC_OP DEC_OP LE_OP GE_OP EQ_OP NE_OP AND_OP OR_OP ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN

%left '-'
%nonassoc UMINUS

%left '+'
%nonassoc UPLUS

%left '/' '*' '%' '<' LE_OP '>' GE_OP EQ_OP NE_OP AND_OP OR_OP ','
%right '!' '=' ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN

%define parse.error verbose

%{
#include <iostream>
#include <vector>
#include <string>
#include <cstddef>

#include "Element.h"
#include "Statement.h"
#include "Expression.h"

extern int yylex();
extern int yyparse();
void yyerror(const char *p)
{
	std::cout << "Error: " << p << std::endl;
}

Document doc;
%}

%union {
	std::string *string;
	int64_t integer;
	char character;
	Type type;
	std::vector<Node *> *list;
	Node *node;

	Element *element;
	Document *doc;
}

%type <doc> document
%type <node> document_element fonction_appel lvalue expr decl_var decl decl_1 decl_2 decl_uniq func_decl any_decl func_def block_item block_item_list block if_decl else if expr_for iter return_stat stat
%type <integer> CONSTANT_INTEGER
%type <character> CONSTANT_CHARACTER
%type <type> TYPE
%type <list> decl_args decl_args_list
%type <string> IDENTIFIER ident


%start document
%%

document
	: document_element           { $$ = &doc; $$->addElement($1); }
	| document document_element  { $1->addElement($2); }

document_element
	: any_decl
	| func_def

ident
	: IDENTIFIER              { $$ = new std::string($1); }

fonction_appel
	: ident '(' expr ')' { $$ = new FunctionAppel($1, $3); }
	| ident '(' ')'      { $$ = new FunctionAppel($1); }

lvalue
	: ident { $$ = new Variable($1); }
	| ident '[' expr ']' { $$ = new Variable($1, $3); }

expr
	: CONSTANT_INTEGER { $$ = new Const(val); }
	| lvalue
	// | fonction_appel '[' expr ']' // TODO this needs some more designing
	| lvalue INC_OP           { $$ = newAffectationPostInc($1); }
	| lvalue DEC_OP           { $$ = newAffectationPostDec($1); }
	| INC_OP lvalue           { $$ = newAffectationPreInc($2); }
	| DEC_OP lvalue           { $$ = newAffectationPreDec($2); }
	| '(' expr ')'            { $$ = $2; }
	| '+' expr %prec UPLUS      { $$ = new UnaryExpression(POS, $2); }
	| '-' expr %prec UMINUS      { $$ = new UnaryExpression(NEG, $2); }
	| '!' expr                { $$ = new UnaryExpression(EXCLAMATION, $2); }
	| expr '/' expr           { $$ = new BinaryExpression($1, DIV, $3); }
	| expr '*' expr           { $$ = new BinaryExpression($1, MULT, $3); }
	| expr '%' expr           { $$ = new BinaryExpression($1, MODULO, $3); }
	| expr '+' expr           { $$ = new BinaryExpression($1, PLUS, $3); }
	| expr '-' expr           { $$ = new BinaryExpression($1, MINUS, $3); }
	| expr '<' expr           { $$ = new BinaryExpression($1, LT, $3); }
	| expr LE_OP expr         { $$ = new BinaryExpression($1, LE, $3); }
	| expr '>' expr           { $$ = new BinaryExpression($1, GT, $3); }
	| expr GE_OP expr         { $$ = new BinaryExpression($1, GE, $3); }
	| expr EQ_OP expr         { $$ = new BinaryExpression($1, EQ, $3); }
	| expr NE_OP expr         { $$ = new BinaryExpression($1, NE, $3); }
	| expr AND_OP expr        { $$ = new BinaryExpression($1, AND, $3); }
	| expr OR_OP expr         { $$ = new BinaryExpression($1, OR, $3); }
	| lvalue '=' expr         { $$ = new Affectation($1, $3); }
	| lvalue ADD_ASSIGN expr  { $$ = new Affectation($1, new BinaryExpression($1, PLUS, $3)); }
	| lvalue SUB_ASSIGN expr  { $$ = new Affectation($1, new BinaryExpression($1, MINUS, $3)); }
	| lvalue MUL_ASSIGN expr  { $$ = new Affectation($1, new BinaryExpression($1, MULT, $3)); }
	| lvalue DIV_ASSIGN expr  { $$ = new Affectation($1, new BinaryExpression($1, DIV, $3)); }
	| lvalue MOD_ASSIGN expr  { $$ = new Affectation($1, new BinaryExpression($1, MODULO, $3)); }
	| fonction_appel          { $$ = $1; }
	| expr ',' expr           { $$ = new BinaryExpression($1, COMMA, $3); }

decl_var
	: ident { $$ = new VarDecl($1, 1); }
	| ident '[' CONSTANT_INTEGER ']' { $$ = new VarDecl($1, $3); }
	| ident '['  ']' { $$ = new VarDecl($1, 0); } // new declaration of array with currently unknown size, this must be resolved by a initialiser list

decl
	: TYPE decl_1 ';' { $$ = new VarDeclList($1, $2); }

decl_1
	: decl_2 { $$ = new std::vector<Element *>; $$->push_back($1); }
	| decl_1 ',' decl_2 { $1->push_back($3); }

decl_2
	: decl_var { $$ = $1; }
	| decl_var '=' expr { $$ = new VarDef($1, PLACEHOLDER_TYPE, $3); }

decl_uniq
	: TYPE decl_var { $$ = $2; $2->updateType($1); }

decl_args_list
	: TYPE { $$ = new std::vector<Element *>; $$->push_back(new VarDecl("unnamed_variable", $1)); }
	| decl_uniq { $$ = new std::vector<Element *>; $$->push_back($1); }
	| decl_args_list ',' TYPE { $1->push_back(new VarDecl("", $3)); }
	| decl_args_list ',' decl_uniq { $1->push_back($3); }

decl_args
	: '('')'                  { $$ = new std::vector<Element *>; }
	| '(' decl_args_list ')'  { $$ = $2; }

func_decl
	: TYPE ident decl_args ';' { $$ = new FuncDecl($2, $1, $3); }

any_decl
	: decl
	| func_decl

func_def
	: TYPE ident decl_args block { $$ = new FuncDef($2, $1, $3, $4); }

block_item
	: decl
	| stat

block_item_list
	: block_item { $$ = new std::vector<Element *>; $$->push_back($1); }
	| block_item_list block_item { $1->push_back($2); }

block
	: '{' '}' { $$ = new Block; }
	| '{' block_item_list '}' { $$ = new Block($2); }

if_decl
	: IF '(' expr ')' stat { $$ = new Cond($5, $3); }

else
	: /* vide */ { $$ = nullptr; }
	| ELSE stat  { $$ = $2; }

if
	: if_decl else { $1->updateElse($2); }

expr_for
	: /* vide */ { $$ = nullptr; }
	| expr

iter
	: WHILE '(' expr ')' stat { $$ = new Iter($5, $3); }
	| FOR '(' expr_for ';' expr_for ';' expr_for ')' stat { $$ = new Iter($9, $5); } // TODO new class needed

return_stat
	: RETURN ';' { $$ = new Return; }
	| RETURN expr ';' { $$ = new Return(expr); }

stat
	: if
	| iter
	| block
	| expr ';'
	| ';' { $$ = new NullStatement; } // TODO or just use nullptr ?
	| return_stat


%%
int main(int argc, char **argv)
{
	yyparse();
}
