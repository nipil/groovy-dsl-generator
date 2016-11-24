#ifndef DSLGEN_H
#define DSLGEN_H

#include <fstream>

#include "myparser.h"

using namespace std;

class DslGen {

private:
    const MyParser& parser;
    string packagePath;
    set<string> usedCustomTypes;

    string getClassName(string customTypeName) const;

    void createOutputDirectory() const;
    void generatePackage(ostream *out) const;
    void generateSpecifications();
    void generateSpecification(const MyParser::SpecDef* const spec) const;
    void generateDefinition(ostream* out, MyParser::DslDef* def) const;

    ofstream* createOutFile(const string& filepath) const;
    ofstream* createClassFile(const string& classname) const;

    void scanSpecForMembers(const MyParser::SpecDef* const spec);

public:
    DslGen(const MyParser& parser);

    void generate();
};

#endif
