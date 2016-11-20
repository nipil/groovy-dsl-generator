#include <iostream>
#include <cstdlib>

#include "heading.h"

MyParser::MyParser() {
}

bool MyParser::hasCustomType(string type) const {
    return this->custom_types.find(type) != this->custom_types.end();
}

void MyParser::addCustomType(string type) {
    cout << "\tAdding custom type: " << type << endl;
    this->custom_types.insert(type);
}

void MyParser::setPackage(string pkg) {
    cout << "\tSetting package path: " << pkg << endl;
    this->package = pkg;
}

MyParser::Type* MyParser::standardType_to_typeDeclaration(string* standardType) const {
    // get provided type
    MyParser::Type* type = standardType;
    // debug output
    cout << "type_standard " << *type << " at " << type << endl;
    // forward type
    return type;
}

MyParser::Type* MyParser::customType_to_typeDeclaration(string* customType) {
    // get provided type
    MyParser::Type* type = customType;
    // check that type has been defined
    if (!this->hasCustomType(*type)) {
        cerr << "type unknown ! " << *type << endl;
        exit(1);
    }
    // debug output
    cout << "type_custom " << *type << " at " << type << endl;
    // forward type
    return type;
}

MyParser::TypeList* MyParser::typeDeclarations_createfor_typeDeclaration(MyParser::Type* type) const {
    // build list
    MyParser::TypeList* lst = new MyParser::TypeList();
    // add provided type
    lst->push_back(type);
    // debug output
    cout << " type_declaration (alone) " << *type << " at " << type << " into " << lst << endl;
    // forward list
    return lst;
}

MyParser::TypeList* MyParser::typedeclarations_add_typeDeclaration(MyParser::TypeList* lst, MyParser::Type* type) const {
    // add latest provided type to list
    lst->push_back(type);
    // debug output
    cout << " type_declaration (multiple) " << *type << " at " << type << " into " << lst << endl;
    // forward list
    return lst;
}
