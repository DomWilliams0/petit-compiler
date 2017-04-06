N [0-9]
IC [a-zA-Z_]

%{
#include "Element.h"
#include "Statement.h"
#include "Expression.h"
#include "c.tab.h"

#undef yywrap
extern "C" {
	int yywrap()
	{
		return 1;
	}
}

extern unsigned int errorCount;

int yyerror(const char *error)
{
	errorCount += 1;
	std::cerr << "Syntax error at line " << yylineno << " " << error << ": " << yytext << std::endl;
	return 0;
}
%}

%option noyywrap
%option yylineno
%option nounput


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

"'"."'" { yylval.character = yytext[1]; return CONSTANT_CHARACTER; }
"'\\"."'" {
	switch(yytext[2])
	{
		case 'a':
			yylval.character = '\a';
			break;

		case 'b':
			yylval.character = '\b';
			break;

		case 'f':
			yylval.character = '\f';
			break;

		case 'n':
			yylval.character = '\n';
			break;

		case 'r':
			yylval.character = '\r';
			break;

		case 't':
			yylval.character = '\t';
			break;

		case 'v':
			yylval.character = '\v';
			break;

		case '\\':
			yylval.character = '\\';
			break;

		case '\'':
			yylval.character = '\'';
			break;

		case '"':
			yylval.character = '\"';
			break;
		default:
			yyerror("Unknown character");
			break;

	}
	return CONSTANT_CHARACTER; }

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

. { yyerror("Unexpected token"); }
%%
