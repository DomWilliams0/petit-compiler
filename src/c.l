N [0-9]
IC [a-zA-Z_]


%{
#include <iostream>
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
"int32_t" { return TYPE; }
"int64_t" { return TYPE; }
"char" { return TYPE; }
"void" { return TYPE; }

{N}+ { return CONSTANT; }
\\?{N} { return CONSTANT; }
{IC}({IC}|{N})* { return IDENTIFIER; }

"'"\\?."'" { return CONSTANT; }

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
