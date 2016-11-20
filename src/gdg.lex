/* this is the lexer file producing tokens */
/* compile to C program using `flex` */

%{
#include "heading.h"
#include "gdg.tab.h"
int yyerror(char *s);
%}

digit       [0-9]
letter      [a-zA-Z]
name        {letter}({digit}|{letter}|_)*

package_litteral     {name}("."{name})*

standard_type_litteral ("num"|"txt")

%%

";" {
    printf("%s\n", yytext);
    return STATEMENT_END;
}

"@" {
    printf("%s", yytext);
    return PACKAGE_DECLARATOR;
}

"%" {
    printf("%s", yytext);
    return TYPE_DECLARATOR;
}

"{" {
    printf("%s", yytext);
    return CLOSURE_START;
}

"}" {
    printf("%s", yytext);
    return CLOSURE_END;
}

{name} {
    printf("name %s ", yytext);
    return NAME;
}

{package_litteral} {
    printf("package %s ", yytext);
    return PACKAGE_NAME;
}

[ \t]*      ;
[\n]        { yylineno++; }

. { printf("unknown %s ", yytext); }

%%
