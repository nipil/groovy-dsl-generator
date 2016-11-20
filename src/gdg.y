/* gdg.y */

%{
#include "heading.h"
int yyerror(char *s);
int yylex(void);
%}

%union{
  int int_val;
  string* str_val;
}

/* points to the "top-level" (aka whole document) rule */
%start input

%token <str_val> PACKAGE
%token <str_val> IDENTIFIER
%token <str_val> CUSTOM_TYPE
%token <str_val> STANDARD_TYPE
%token CLOSURE_START
%token CLOSURE_END
%token TYPE_SEPARATOR
%token STATEMENT_END

/* nicer error messages ("end of file" instead of "$end") */
%token END 0 "end of file"

%%

input: PACKAGE statements
     { cout << "package " << *$1 << endl; }
     ;

statements: statement
          | statements statement
          ;

statement: spec_declaration
         | dsl_definition
         ;

spec_declaration: CUSTOM_TYPE CLOSURE_START dsl_definitions CLOSURE_END
                { cout << "spec_declaration " << *$1 << endl; }
                ;

dsl_definitions: dsl_definition
               | dsl_definitions dsl_definition
               ;

dsl_definition: IDENTIFIER type_declarations
              { cout << "dsl_definition " << *$1 << endl; }
              ;

type_declarations: type_declaration
                 | type_declarations TYPE_SEPARATOR type_declaration
                 ;

type_declaration: STANDARD_TYPE
                  { cout << "type_standard " << *$1 << endl; }
                | CUSTOM_TYPE
                  { cout << "custom_type " << *$1 << endl; }
                ;
%%

int yyerror(string s)
{
  extern int yylineno;  // defined and maintained in lex.c
  extern char *yytext;  // defined and maintained in lex.c

  cerr << "ERROR: " << s << " at symbol \"" << yytext;
  cerr << "\" on line " << yylineno << endl;
  exit(1);
}

int yyerror(char *s)
{
  return yyerror(string(s));
}
