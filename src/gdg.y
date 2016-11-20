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
  list<string*>* strlist_val;
}

/* points to the "top-level" (aka whole document) rule */
%start input

%token <str_val> PACKAGE
%token <str_val> IDENTIFIER
%token <str_val> CUSTOM_TYPE
%token <str_val> STANDARD_TYPE
%type <str_val> type_declaration
%type <strlist_val> type_declarations
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
            cout << "      spec_declaration (double)" << endl;
          }
          ;

statement: spec_declaration {
           cout << "     statement (spec_decl)" << endl;
         }
         | dsl_definition {
           cout << "     statement (dsl_def)" << endl;
         }
         ;

spec_declaration: CUSTOM_TYPE CLOSURE_START dsl_definitions CLOSURE_END {
                  cout << "    spec_declaration " << *$1 << " at " << $1 << endl;
                  myparser.addCustomType(*$1);
                }
                ;

dsl_definitions: dsl_definition {
                  cout << "   dsl_definitions (alone)" << endl;
               }
               | dsl_definitions dsl_definition {
                  cout << "   dsl_definitions (multiple)" << endl;
               }
               ;

dsl_definition: IDENTIFIER type_declarations
              { cout << "  dsl_definition '" << *$1 << "'" << endl; }
              ;

type_declarations: type_declaration {
                  list<string*>* lst = new list<string*>();
                  lst->push_back($1);
                  cout << " type_declaration (alone) " << *$1 << " at " << $1 << " into " << lst << endl;
                  $$ = lst;
                 }
                 | type_declarations TYPE_SEPARATOR type_declaration {
                  list<string*>* lst = $1;
                  lst->push_back($3);
                  cout << " type_declaration (multiple) " << *$3 << " at " << $3 << " into " << lst << endl;
                 }
                 ;

type_declaration: STANDARD_TYPE {
                  cout << "type_standard " << *$1 << " at " << $1 << endl;
                }
                | CUSTOM_TYPE {
                  cout << "type_custom " << *$1 << " at " << $1 << endl;
                  if (!myparser.hasCustomType(*$1)) {
                    cerr << "type unknown ! " << $1 << endl;
                    exit(1);
                  }
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
