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
custom_type "%"{identifier}
package     "@"{identifier}("."{identifier})*

%%

{package} {
    // printf("pkg %s\n", yytext);
    // remove package marker '@'
    char * package_name = yytext + 1;
    yylval.str_val = new string(package_name);
    return PACKAGE;
}

{std_type} {
    // printf("std %s\n", yytext);
    yylval.str_val = new string(yytext);
    return STANDARD_TYPE;
}

{custom_type} {
    // printf("custom %s\n", yytext);
    char * type_name = yytext + 1;
    yylval.str_val = new string(type_name);
    return CUSTOM_TYPE;
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
