#ifndef DSLGEN_H
#define DSLGEN_H

#include <sstream>

#include "myparser.h"

using namespace std;

class DslGen {

private:
    const MyParser& parser;

public:

public:
    DslGen(const MyParser& parser);

    void generate();
};

#endif
