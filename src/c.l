N [0-9]
IC [a-zA-Z_]


%{
#include "Element.h"
#include "Statement.h"
#include "Expression.h"
#include "c.tab.h"
%}

%option noyywrap

%%

"#"[^\n]* { /* ignore */ }
"//"[^\n]* { /* ignore */ }
[\n\t\r ] { /* ignore */ }

"if" { return IF; }
"else" { return ELSE; }
"while" { return WHILE; }
"for" { return FOR; }

"return" { return RETURN; }
"int32_t" { yylval.type = INT32; return TYPE; }
"int64_t" { yylval.type = INT64; return TYPE; }
"char" { yylval.type = CHAR; return TYPE; }
"void" { yylval.type = VOID; return TYPE; }

{N}+ { yylval.integer = atoi(yytext); return CONSTANT_INTEGER; }
{IC}({IC}|{N})* { yylval.string = new std::string(yytext); return IDENTIFIER; }

"'"."'" { yylval.character = yytext[1]; return CONSTANT_CHARACTER; } // TODO allow escaped characters

"++" { return INC_OP; }
"--" { return DEC_OP; }
"<=" { return LE_OP; }
"=>" { return GE_OP; }
"==" { return EQ_OP; }
"!=" { return NE_OP; }
"&&" { return AND_OP; }
"||" { return OR_OP; }
"+=" { return ADD_ASSIGN; }
"-=" { return SUB_ASSIGN; }
"*=" { return MUL_ASSIGN; }
"/=" { return DIV_ASSIGN; }
"%=" { return MOD_ASSIGN; }

";" { return ';'; }
"{" { return '{'; }
"}" { return '}'; }
"," { return ','; }
"=" { return '='; }
"(" { return '('; }
")" { return ')'; }
"[" { return '['; }
"]" { return ']'; }
"!" { return '!'; }
"-" { return '-'; }
"+" { return '+'; }
"*" { return '*'; }
"/" { return '/'; }
"%" { return '%'; }
"<" { return '<'; }
">" { return '>'; }
%%
