#ifndef MYPARSER_H
#define MYPARSER_H

#include <set>
#include <list>
#include <map>

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

    typedef set<string> CustomTypes;
    typedef map<Type,SpecDef*> Specifications;
    typedef map<string,DslDef*> Definitions;

private:
    CustomTypes custom_types;
    Specifications specifications;
    Definitions definitions;
    string package;

public:
    MyParser();

    const Specifications& getSpecifications() const;
    const Definitions& getDefinitions() const;
    const string& getPackage() const;

    bool hasCustomType(string type) const;
    void addCustomType(string type);
    void setPackage(string pkg);
    void addSpecification(SpecDef* spec);
    void addDefinition(DslDef* dsl);

    MyParser::Type* standardType_to_typeDeclaration(string* standardType) const;
    MyParser::Type* customType_to_typeDeclaration(string* standardType) const;
    MyParser::TypeList* typeDeclarations_createfor_typeDeclaration(MyParser::Type* type) const;
    MyParser::TypeList* typeDeclarations_add_typeDeclaration(MyParser::TypeList* lst, MyParser::Type* type) const;
    MyParser::DslDef* createDslDefinition(string* dslKeyword, MyParser::TypeList* lst) const;
    MyParser::DslDefList* dslDefinitions_createfor_dslDefinition(MyParser::DslDef* dslDef) const;
    MyParser::DslDefList* dslDefinitions_add_dslDefinition(MyParser::DslDefList* lst, MyParser::DslDef* dslDef) const;
    MyParser::SpecDef* createSpecDefinition(string* typeKeyword, MyParser::DslDefList* lst);

    void display() const;
};

#endif
