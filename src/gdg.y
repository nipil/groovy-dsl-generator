/* gdg.y */

%{
#include "heading.h"
int yyerror(char *s);
int yylex(void);
MyParser myparser;
%}

%union{
  int int_val;
  string* str_val;
  MyParser::Type* type_val;
  MyParser::TypeList* typelist_val;
  MyParser::DslDef* dsldef_val;
  MyParser::DslDefList* dsldeflist_val;
  MyParser::SpecDef* specdef_val;
}

/* points to the "top-level" (aka whole document) rule */
%start input

%token <str_val> PACKAGE
%token <str_val> IDENTIFIER
%token <str_val> CUSTOM_TYPE
%token <str_val> STANDARD_TYPE
%type <type_val> type_declaration
%type <typelist_val> type_declarations
%type <dsldef_val> dsl_definition
%type <dsldeflist_val> dsl_definitions
%type <specdef_val> spec_declaration
%token CLOSURE_START
%token CLOSURE_END
%token TYPE_SEPARATOR
%token STATEMENT_END

/* nicer error messages ("end of file" instead of "$end") */
%token END 0 "end of file"

%%

input: PACKAGE statements {
        cout << "       input" << endl;
        myparser.setPackage(*$1);
     }
     ;

statements: statement {
            cout << "      spec_declaration (alone)" << endl;
          }
          | statements statement {
            cout << "      spec_declaration (multiple)" << endl;
          }
          ;

statement: spec_declaration {
           myparser.addSpec($1);
         }
         | dsl_definition {
           myparser.addDef($1);
         }
         ;

spec_declaration: CUSTOM_TYPE CLOSURE_START dsl_definitions CLOSURE_END {
                  $$ = myparser.createSpecDefinition($1, $3);
                }
                ;

dsl_definitions: dsl_definition {
                $$ = myparser.dslDefinitions_createfor_dslDefinition($1);
               }
               | dsl_definitions dsl_definition {
                $$ = myparser.dslDefinitions_add_dslDefinition($1, $2);
               }
               ;

dsl_definition: IDENTIFIER type_declarations {
                $$ = myparser.createDslDefinition($1, $2);
              }
              ;

type_declarations: type_declaration {
                  $$ = myparser.typeDeclarations_createfor_typeDeclaration($1);
                 }
                 | type_declarations TYPE_SEPARATOR type_declaration {
                  $$ = myparser.typedeclarations_add_typeDeclaration($1, $3);
                 }
                 ;

type_declaration: STANDARD_TYPE {
                  $$ = myparser.standardType_to_typeDeclaration($1);
                }
                | CUSTOM_TYPE {
                  $$ = myparser.customType_to_typeDeclaration($1);
                }
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
