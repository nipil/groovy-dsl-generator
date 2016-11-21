#ifndef MYPARSER_H
#define MYPARSER_H

#include <set>
#include <list>

using namespace std;

class MyParser {

public:
    typedef list<string*> TypeList;
    typedef string Type;
    typedef struct {
        string* keyword;
        TypeList* types;
    } DslDef;
    typedef list<DslDef*> DslDefList;
    typedef struct {
        Type* type;
        DslDefList* defs;
    } SpecDef;

private:
    set<string> custom_types;
    string package;

public:
    MyParser();
    bool hasCustomType(string type) const;
    void addCustomType(string type);
    void setPackage(string pkg);

    MyParser::Type* standardType_to_typeDeclaration(string* standardType) const;
    MyParser::Type* customType_to_typeDeclaration(string* standardType) const;
    MyParser::TypeList* typeDeclarations_createfor_typeDeclaration(MyParser::Type* type) const;
    MyParser::TypeList* typedeclarations_add_typeDeclaration(MyParser::TypeList* lst, MyParser::Type* type) const;
    MyParser::DslDef* createDslDefinition(string* dslKeyword, MyParser::TypeList* lst) const;
    MyParser::DslDefList* dslDefinitions_createfor_dslDefinition(MyParser::DslDef* dslDef) const;
    MyParser::DslDefList* dslDefinitions_add_dslDefinition(MyParser::DslDefList* lst, MyParser::DslDef* dslDef) const;
    MyParser::SpecDef* createSpecDefinition(string* typeKeyword, MyParser::DslDefList* lst);
};

#endif
