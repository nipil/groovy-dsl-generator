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

";" {
    // printf("%s\n", yytext);
    return STATEMENT_END;
}

"{" {
    // printf("%s\n", yytext);
    return CLOSURE_START;
}

"}" {
    // printf("%s\n", yytext);
    return CLOSURE_END;
}

"," {
    // printf("%s\n", yytext);
    return TYPE_SEPARATOR;
}

{package} {
    // printf("pkg %s\n", yytext);
    yylval.str_val = new string(yytext);
    return PACKAGE;
}

{std_type} {
    // printf("std %s\n", yytext);
    yylval.str_val = new string(yytext);
    return STANDARD_TYPE;
}

{custom_type} {
    // printf("custom %s\n", yytext);
    yylval.str_val = new string(yytext);
    return CUSTOM_TYPE;
}

{identifier} {
    // printf("id %s\n", yytext);
    yylval.str_val = new string(yytext);
    return IDENTIFIER;
}


[ \t]*      ;
[\n]        { yylineno++; }

. { printf("unknown %s\n", yytext); }

%%
