/* this is the lexer file producing tokens */
/* compile to C program using `flex` */

%{
#include <stdio.h>
%}

DIGIT [[:digit:]]
LETTER [[:alpha:]]
WORD {LETTER}+
NAME {LETTER}({LETTER}|{DIGIT}|_)*

PACKAGE "@"{NAME}("."{NAME})*

TYPE_STANDARD (num|txt)
TYPE_SEPARATOR ","
TYPE_CUSTOM "%"{NAME}

CLOSURE_OPEN "{"
CLOSURE_CLOSE "}"

STATEMENT_TERMINATOR ";"

%%

{PACKAGE} { printf("found package keyword\n"); }

{TYPE_STANDARD} { printf("found standard type %s\n", yytext); }
{TYPE_SEPARATOR} { printf("found type separator %s\n", yytext); }
{TYPE_CUSTOM} { printf("found custom type %s\n", yytext); }

{CLOSURE_OPEN} { printf("found closure start\n"); }
{CLOSURE_CLOSE} { printf("found closure end\n"); }

{NAME} { printf("found name %s\n", yytext); }

{STATEMENT_TERMINATOR} { printf("found statement_terminator\n"); }

[ \t\n\r]+ ;

. { printf("unknown character %s\n", yytext); }

%%

int main(int argc, char** argv) {
    yylex();
    return 0;
}
