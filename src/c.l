N [0-9]
IC [a-zA-Z_]


%{
#include "Element.h"
#include "Statement.h"
#include "Expression.h"
#include "c.tab.h"

// ce fichier .l ne fera pas appel a la methode yywrap(), cad que notre lexeur genere ne lex qu'1 seul fichier a chaque execution.
%}

%option noyywrap

%%

"#"[^\n]* { /* ignore - pour tous caracteres suivant un # et qui ne sont pas \n, on les ignore( on ignore aussi le # */ }
"//"[^\n]* { /* ignore - pour tous caracteres suivant un // et qui ne sont pas \n, on les ignore( on ignore aussi le */ }
[\n\t\r ] { /* ignore - on ignore \n(changer de ligne), \t(saut) et \r(entrer) */ }

"if" { return IF; }
"else" { return ELSE; }
"while" { return WHILE; }
"for" { return FOR; }

"return" { return RETURN; }
"int32_t" { yylval.type = INT32; return TYPE; }
"int64_t" { yylval.type = INT64; return TYPE; }
"char" { yylval.type = CHAR; return TYPE; }
"void" { yylval.type = VOID; return TYPE; }


{N}+ { yylval.integer = atoi(yytext); return CONSTANT_INTEGER; /* pour toutes les suites de chiffres, on renvoie une valeur et CONSTANT_INTEGER yyval stocke la valeur correspondante d'un jeton(token) par defaut, yylval est de type int(ici on veut integer */ }

{IC}({IC}|{N})* { yylval.string = new std::string(yytext); return IDENTIFIER; /* pour toutes les suites commancant avec une lettre et suivies par des lettres et/ou des chiffres, on les consideres comme un string */ }

"'"."'" { yylval.character = yytext[1]; return CONSTANT_CHARACTER; /* TODO escape characters */ }

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
