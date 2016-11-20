/* gdg.y */

%{
#include "heading.h"
int yyerror(char *s);
int yylex(void);
%}

%union{
  int		int_val;
  string*	str_val;
}


/* points to the "top-level" (aka whole document) rule */
%start input

%token <str_val> NAME

%token PACKAGE_DECLARATOR
%token <str_val> PACKAGE_NAME

%token TYPE_DECLARATOR
%token CLOSURE_START
%token CLOSURE_END

%token <str_val> STANDARD_TYPE
%token TYPE_SEPARATOR

%token STATEMENT_END

/* nicer error messages ("end of file" instead of "$end") */
%token END 0 "end of file"

%%

input: package_decl statement
		;

statement: /* empty */
    |      type_decl
    |      content_decl
    ;

package_decl: PACKAGE_DECLARATOR PACKAGE_NAME STATEMENT_END
    ;

type_decl: TYPE_DECLARATOR NAME CLOSURE_START dsl_decl CLOSURE_END STATEMENT_END
    ;

dsl_decl: /* empty */
    |     decl_statement
    |     dsl_decl dsl_decl
    ;

decl_statement: NAME type_statement
    ;

type_statement: type_name
    |           type_statement TYPE_SEPARATOR type_statement
    ;

type_name: STANDARD_TYPE
    |      NAME
    ;

content_decl: NAME STANDARD_TYPE
    ;

%%

int yyerror(string s)
{
  extern int yylineno;	// defined and maintained in lex.c
  extern char *yytext;	// defined and maintained in lex.c

  cerr << "ERROR: " << s << " at symbol \"" << yytext;
  cerr << "\" on line " << yylineno << endl;
  exit(1);
}

int yyerror(char *s)
{
  return yyerror(string(s));
}


