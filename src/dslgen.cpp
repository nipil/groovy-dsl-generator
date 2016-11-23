#include <cstdlib>

#include <sstream>
#include <iostream>
#include <algorithm>

#include "myparser.h"
#include "dslgen.h"

DslGen::DslGen(const MyParser& myparser)
: parser(myparser) {
}

void DslGen::generate() {
	this->packagePath = parser.getPackage();
	replace(this->packagePath.begin(), this->packagePath.end(), '.', '/');
	this->createOutputDirectory();
	this->generateSpecifications();
}

void DslGen::createOutputDirectory() const {
	cout << "Creating folder " << this->packagePath << endl;
	int result = system(string("mkdir -p " + this->packagePath).c_str());
	if (result != 0) {
		cout << "error while creating directory" << endl;
		exit(1);
	}
}

void DslGen::generateSpecifications() {
	const MyParser::Specifications& specs = parser.getSpecifications();
	for (MyParser::Specifications::const_iterator it = specs.begin(); it != specs.end(); it++) {
		MyParser::SpecDef* spec = it->second;
		cout << "Generating class for spec: " << *spec->type << endl;
	}
}
