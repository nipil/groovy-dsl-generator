#ifndef MYPARSER_H
#define MYPARSER_H

#include <set>
#include <list>

using namespace std;

class MyParser {

public:
    typedef list<string*> TypeList;
    typedef string Type;

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
