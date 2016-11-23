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
}

void DslGen::createOutputDirectory() const {
	cout << "Creating folder " << this->packagePath << endl;
	int result = system(string("mkdir -p " + this->packagePath).c_str());
	if (result != 0) {
		cout << "error while creating directory" << endl;
		exit(1);
	}
}
