#ifndef DSLGEN_H
#define DSLGEN_H

#include <sstream>

#include "myparser.h"

using namespace std;

class DslGen {

private:
    const MyParser& parser;
    string packagePath;

    void createOutputDirectory() const;
    void generateSpecifications();

public:
    DslGen(const MyParser& parser);

    void generate();
};

#endif
