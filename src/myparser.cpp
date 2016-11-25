#include <sstream>

#include "heading.h"

extern int yylineno;  // defined and maintained in lex.c

MyParser::MyParser() {
}

bool MyParser::hasCustomType(string type) const {
    return this->custom_types.find(type) != this->custom_types.end();
}

void MyParser::addCustomType(string type) {
    // cout << "\tAdding custom type: " << type << endl;
    if (this->hasCustomType(type)) {
        cerr << "ERROR on line " << yylineno << ": Custom type " << type << " already declared" << endl;
        exit(1);
    }
    this->custom_types.insert(type);
}

void MyParser::setPackage(string pkg) {
    // cout << "\tSetting package path: " << pkg << endl;
    this->package = pkg;
}

MyParser::Type* MyParser::standardType_to_typeDeclaration(string* standardType) const {
    // get provided type
    MyParser::Type* type = standardType;
    // debug output
    // cout << "type_standard " << *type << " at " << type << endl;
    // forward type
    return type;
}

MyParser::Type* MyParser::customType_to_typeDeclaration(string* customType) const {
    // get provided type
    MyParser::Type* type = customType;
    // check that type has been defined
    if (!this->hasCustomType(*type)) {
        cerr << "ERROR on line " << yylineno << ": Custom type " << *type << " unknown (not declared)" << endl;
        exit(1);
    }
    // debug output
    // cout << "type_custom " << *type << " at " << type << endl;
    // forward type
    return type;
}

MyParser::TypeList* MyParser::typeDeclarations_createfor_typeDeclaration(MyParser::Type* type) const {
    // build list
    MyParser::TypeList* lst = new MyParser::TypeList();
    // add provided type
    lst->push_back(type);
    // debug output
    // cout << " type_declaration (alone) " << *type << " at " << type << " into " << lst << endl;
    // forward list
    return lst;
}

MyParser::TypeList* MyParser::typeDeclarations_add_typeDeclaration(MyParser::TypeList* lst, MyParser::Type* type) const {
    // check start closure with something behind
    if (this->hasCustomType(**lst->begin())) {
        cerr << "ERROR on line " << yylineno << ": cannot have multiple custom types on same definition without re-introducing parenthesis in target dsl text" << endl;
        exit(1);
    }
    // add latest provided type to list
    lst->push_back(type);
    // debug output
    // cout << " type_declaration (multiple) " << *type << " at " << type << " into " << lst << endl;
    // forward list
    return lst;
}

MyParser::DslDef* MyParser::createDslDefinition(string* dslKeyword, MyParser::TypeList* lst) const {
    // build dsl definition entry
    MyParser::DslDef* dslDef = new MyParser::DslDef();
    dslDef->keyword = dslKeyword;
    dslDef->types = lst;
    // debug output
    // cout << "  dsl_definition '" << *dslKeyword << "' at " << dslDef << " typed:";
    // for (MyParser::TypeList::iterator it = lst->begin(); it != lst->end(); it++) {
    //     MyParser::Type* t = *it;
    //     cout << " " << *t;
    // }
    // cout << endl;
    // forward definition
    return dslDef;
}

MyParser::DslDefList* MyParser::dslDefinitions_createfor_dslDefinition(MyParser::DslDef* dslDef) const {
    // build list
    MyParser::DslDefList* lst = new MyParser::DslDefList();
    // add provided dsl definition
    lst->push_back(dslDef);
    // debug output
    // cout << "   dsl_definitions (alone) created at " << lst << " from " << dslDef << endl;
    // forward list
    return lst;
}

MyParser::DslDefList* MyParser::dslDefinitions_add_dslDefinition(MyParser::DslDefList* lst, MyParser::DslDef* dslDef) const {
    // add provided dsl definition
    lst->push_back(dslDef);
    // debug output
    // cout << "   dsl_definitions (multiple) at " << lst << " added " << dslDef << endl;
    // forward list
    return lst;
}

MyParser::SpecDef* MyParser::createSpecDefinition(string* typeKeyword, MyParser::DslDefList* lst) {
    // add custom type to definition
    MyParser::Type* type = typeKeyword;
    this->addCustomType(*type);
    // build spec definition entry
    MyParser::SpecDef* specDef = new MyParser::SpecDef();
    specDef->type = type;
    specDef->defs = lst;
    // debug output
    // cout << "    spec_declaration " << *type << " created " << specDef << " from type " << type << " dsldef " << lst << endl;
    // forward spec
    return specDef;
}

void MyParser::addSpecification(SpecDef* spec) {
    // cout << "\tNew closure spec: " << *spec->type << endl;
    this->specifications[*spec->type] = spec;
}

void MyParser::addDefinition(DslDef* dsl) {
    stringstream ss;
    ss << *dsl->keyword << "(";
    for (TypeList::const_iterator it = dsl->types->begin(); it != dsl->types->end(); it++) {
        if (it != dsl->types->begin()) {
            ss << ",";
        }
        ss << **it;
    }
    ss << ")";
    string str = ss.str();
    // cout << "New dsl definition: " << str << endl;
    this->definitions[str] = dsl;
}

void MyParser::display() const {
    cout << "Target package : " << this->package << endl;
    cout << "Parsed " << this->specifications.size() << " closure declaration:";
    for (Specifications::const_iterator it = this->specifications.begin(); it != this->specifications.end(); it++) {
        cout << " " << it->first;
    }
    cout << endl;
    cout << "Parsed " << this->definitions.size() << " top-level DSL keywords:";
    for (Definitions::const_iterator it = this->definitions.begin(); it != this->definitions.end(); it++) {
        cout << " " << it->first;
    }
    cout << endl;
}

const MyParser::Specifications& MyParser::getSpecifications() const {
    return this->specifications;
}

const MyParser::Definitions& MyParser::getDefinitions() const {
    return this->definitions;
}

const string& MyParser::getPackage() const {
    return this->package;
}
