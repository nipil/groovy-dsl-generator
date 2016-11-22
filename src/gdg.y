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

/* token, in order of precedence (last is higher precedence) */
%token <str_val> IDENTIFIER
%token <str_val> STANDARD_TYPE
%type <type_val> type_declaration
%type <typelist_val> type_declarations
%type <dsldef_val> dsl_definition
%type <dsldeflist_val> dsl_definitions
%type <specdef_val> spec_declaration
%type <str_val> custom_type
%type <str_val> package_name

/* nicer error messages ("end of file" instead of "$end") */
%token END 0 "end of file"

%%

input: package_statement statements
     ;

package_statement: '@' package_name { myparser.setPackage(*$2); }
                 ;

package_name: IDENTIFIER { $$ = $1; }
            | package_name '.' IDENTIFIER { $1->append("."); $1->append(*$3); $$ = $1; }
            ;

statements: statement
          | statements statement
          ;

statement: spec_declaration { myparser.addSpecification($1); }
         | dsl_definition { myparser.addDefinition($1);}
         ;

spec_declaration: custom_type '{' dsl_definitions '}' { $$ = myparser.createSpecDefinition($1, $3); }
                ;

custom_type : '%' IDENTIFIER { $$ = $2; }
            ;

dsl_definitions: dsl_definition { $$ = myparser.dslDefinitions_createfor_dslDefinition($1); }
               | dsl_definitions dsl_definition { $$ = myparser.dslDefinitions_add_dslDefinition($1, $2); }
               ;

dsl_definition: IDENTIFIER type_declarations { $$ = myparser.createDslDefinition($1, $2); }
              ;

type_declarations: type_declaration { $$ = myparser.typeDeclarations_createfor_typeDeclaration($1); }
                 | type_declarations ',' type_declaration { $$ = myparser.typedeclarations_add_typeDeclaration($1, $3); }
                 ;

type_declaration: STANDARD_TYPE { $$ = myparser.standardType_to_typeDeclaration($1); }
                | custom_type { $$ = myparser.customType_to_typeDeclaration($1); }
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
