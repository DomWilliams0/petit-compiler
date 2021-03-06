// La definition des jetons. Ici on definit quelles sequences de caracteres sont signficatives
%token CONSTANT_INTEGER CONSTANT_CHARACTER IDENTIFIER
%token IF ELSE WHILE FOR
%token RETURN TYPE
%token INC_OP DEC_OP LE_OP GE_OP EQ_OP NE_OP AND_OP OR_OP ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN

%left ','
%right MOD_ASSIGN DIV_ASSIGN MUL_ASSIGN SUB_ASSIGN ADD_ASSIGN '='
%left OR_OP
%left AND_OP
%left NE_OP EQ_OP
%left GE_OP '>' LE_OP '<'
%left '+' '-'
%left '%' '*' '/'
%right'!'
// TODO ++/-- ?

// La declaration nonassoc montre la non-associativité de left '-' et de left '+'
%nonassoc UMINUS
%nonassoc UPLUS

%define parse.error verbose
%parse-param {Document *doc}

%{
#include <iostream>
#include <vector>
#include <string>
#include <cstddef>

#include "Element.h"
#include "Statement.h"
#include "Expression.h"

extern int yylex();
void yyerror(Document *doc, const char *p)
{
	std::cerr << "Error: " << p << std::endl;
}

unsigned int errorCount = 0;

%}

//la structure UNION de la pile de grammaires. La pile est un tableau statique dont chaque element decrit un element grammatical.
//donc on applique la structure UNION
%union {
	std::string *string;
	int64_t integer;
	char character;
	Type type;
	std::vector<Node *> *nodes;
	std::vector<Element *> *elements;
	Node *node;
	Document *doc;

	Block *block;
	Cond *cond;
	Statement *stat;
	Expression *expr;
	Element *element;
	Variable *var;
	VarDecl *varDecl;
	VarDeclList *varDeclList;
}

//definition de relation de correspondance, entre un nom et un non-terminal
%type <doc> document
%type <element> document_element func_decl func_def any_decl decl_2 decl_uniq
%type <stat> if else iter return_stat stat
%type <expr> expr expr_for fonction_appel
%type <var> lvalue
%type <varDecl> decl_var
%type <block> block
%type <cond> if_decl
%type <node> block_item expr_for_init

%type <varDeclList> decl
%type <nodes> block_item_list
%type <elements> decl_args decl_args_list decl_1

%type <integer> CONSTANT_INTEGER
%type <character> CONSTANT_CHARACTER
%type <type> TYPE
%type <string> IDENTIFIER ident

%start document
%%

//notre programme(DOCUMENT) est compose de DOCUMENT_ELEMENT
document
	: document_element           { $$ = doc; $$->addElement($1); }
	| document document_element  { $1->addElement($2); }

document_element
	: any_decl
	| func_def

ident
	: IDENTIFIER

fonction_appel
	: ident '(' expr ')' { $$ = new FunctionAppel(*$1, $3); delete $1; }
	| ident '(' ')'      { $$ = new FunctionAppel(*$1); delete $1; }

//LVALUE 
lvalue
	: ident { $$ = new Variable(*$1); delete $1; }
	| ident '[' expr ']' { $$ = new Variable(*$1, $3); delete $1; }

expr
	: CONSTANT_INTEGER { $$ = new ConstInteger($1); }
	| CONSTANT_CHARACTER { $$ = new ConstCharacter($1); }
	| lvalue { $$ = (Expression *)$1; }
	| lvalue INC_OP           { $$ = new AffectationIncrement(POST_INC, $1); }
	| lvalue DEC_OP           { $$ = new AffectationIncrement(POST_DEC, $1); }
	| INC_OP lvalue           { $$ = new AffectationIncrement(PRE_INC, $2); }
	| DEC_OP lvalue           { $$ = new AffectationIncrement(PRE_DEC, $2); }
	| '(' expr ')'            { $$ = $2; }
	| '+' expr %prec UPLUS    { $$ = new UnaryExpression(POS, $2); }
	| '-' expr %prec UMINUS   { $$ = new UnaryExpression(NEG, $2); }
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
	| lvalue ADD_ASSIGN expr  { $$ = new AffectationCompound($1, PLUS, $3); }
	| lvalue SUB_ASSIGN expr  { $$ = new AffectationCompound($1, MINUS, $3); }
	| lvalue MUL_ASSIGN expr  { $$ = new AffectationCompound($1, MULT, $3); }
	| lvalue DIV_ASSIGN expr  { $$ = new AffectationCompound($1, DIV, $3); }
	| lvalue MOD_ASSIGN expr  { $$ = new AffectationCompound($1, MODULO, $3); }
	| fonction_appel          { $$ = $1; }
	| expr ',' expr           { $$ = new BinaryExpression($1, COMMA, $3); }

decl_var
	: ident { $$ = new VarDecl(*$1, 1); delete $1; }
	| ident '[' CONSTANT_INTEGER ']' { $$ = new VarDecl(*$1, $3); delete $1;}
	| ident '['  ']' { $$ = new VarDecl(*$1, 0); delete $1; } // new declaration of array with currently unknown size, this must be resolved by a initialiser list

decl
	: TYPE decl_1 ';' { $$ = new VarDeclList($1, $2); }

//une serie de declarations successives est autorisee 
decl_1
	: decl_2 { $$ = new std::vector<Element *>; $$->push_back($1); }
	| decl_1 ',' decl_2 { $1->push_back($3); }

decl_2
	: decl_var { $$ = $1; }
	| decl_var '=' expr { $$ = new VarDef($1, $3); }

decl_uniq
	: TYPE decl_var { $$ = $2; $2->updateType($1); }

decl_args_list
	: TYPE { $$ = new std::vector<Element *>; $$->push_back(new VarDecl(typeToString($1), 1)); }
	| decl_uniq { $$ = new std::vector<Element *>; $$->push_back($1); }
	| decl_args_list ',' TYPE { $1->push_back(new VarDecl("", $3)); }
	| decl_args_list ',' decl_uniq { $1->push_back($3); }

decl_args
	: '('')'                  { $$ = new std::vector<Element *>; }
	| '(' decl_args_list ')'  { $$ = $2; }

func_decl
	: TYPE ident decl_args ';' { $$ = new FuncDecl(*$2, $1, $3); delete $2;}

any_decl
	: decl { $$ = $1; }
	| func_decl

func_def
	: TYPE ident decl_args block { $$ = new FuncDef(*$2, $1, $3, $4); delete $2; }

//element qui peut etre contenu dans un bloc est, soit une declaration de varaible/fonction, soit un "statement"
//la definitiond de "statement" peut etre retrouve tout en bas
block_item
	: decl { $$ = (Node *)$1; }
	| stat { $$ = (Node *)$1; }

block_item_list
	: block_item { $$ = new std::vector<Node *>; $$->push_back($1); }
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

expr_for_init
	: ';' { $$ = nullptr; }
	| expr ';' { $$ = (Node *)$1; }
	| decl { $$ = (Node *)$1; }

expr_for
	: /* vide */ { $$ = nullptr; }
	| expr

iter
	: WHILE '(' expr ')' stat { $$ = new Iter($3, $5); }
	| FOR '(' expr_for_init expr_for ';' expr_for ')' stat { $$ = new For($3, $4, $6, $8); }

return_stat
	: RETURN ';' { $$ = new Return; }
	| RETURN expr ';' { $$ = new Return($2); }

stat
	: if
	| iter
	| block { $$ = (Statement *)$1; }
	| expr ';'  { $$ = (Statement *)$1; }
	| expr error ';'  { $$ = nullptr; errorCount += 1; }
	| error ';' { $$ = nullptr; errorCount += 1; }
	| return_stat
	| error { errorCount += 1; yyerror(doc, "Uh oh"); }
%%
