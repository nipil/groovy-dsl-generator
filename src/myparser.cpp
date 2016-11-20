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
