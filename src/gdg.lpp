/* this is the lexer file producing tokens */
/* compile to C program using `flex` */

%{
#include "heading.h"
#include "gdg.tab.h"
int yyerror(char *s);
%}

digit       [0-9]
letter      [a-zA-Z]
identifier  {letter}({digit}|{letter}|_)*
std_type    "num"|"txt"

%%

{std_type} {
	// printf("std %s\n", yytext);
	yylval.str_val = new string(yytext);
	return STANDARD_TYPE;
}

{identifier} {
    // printf("id %s\n", yytext);
    yylval.str_val = new string(yytext);
    return IDENTIFIER;
}

[ \t]*      ;
[\n]        { yylineno++; }

.           { return yytext[0]; }

%%
