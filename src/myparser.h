#ifndef MYPARSER_H
#define MYPARSER_H

#include <set>

using namespace std;

class MyParser {

private:
    set<string> custom_types;
    string package;

public:
    MyParser();
    bool hasCustomType(string type) const;
    void addCustomType(string type);
    void setPackage(string pkg);
};

#endif
