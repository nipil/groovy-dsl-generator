#include <cstdlib>

#include <locale>
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
		this->generateSpecification(it->second);
	}
}

ofstream* DslGen::createClassFile(const string& classname) const {
	string filepath = this->packagePath + "/" + classname + ".groovy";
	return this->createOutFile(filepath);
}

ofstream* DslGen::createOutFile(const string& filepath) const {
	ofstream* f = new ofstream(filepath.c_str(), ofstream::trunc);
	if (!f->is_open()) {
		cerr << "cannot open output file " << filepath << endl;
		exit(1);
	}
	return f;
}

void DslGen::generatePackage(ostream* out) const {
	*out << "package " << parser.getPackage() << endl << endl;
}

void DslGen::generateDefinition(ostream* out, MyParser::DslDef* def) const {
	// TODO: generate object membur for closures present int definition types
	*out << "\tdef " << *def->keyword << "(";
	// TODO: generate arguments for types
	*out << ") {" << endl;
	// TODO: commands for arguments (println/delegate)
	*out << "\t}" << endl;
}

void DslGen::generateSpecification(const MyParser::SpecDef* const spec) const {
	string classname = *spec->type + "Spec";
	locale loc;
	classname[0] = toupper(classname[0], loc);
	cout << "Generating class " << classname << " for spec " << *spec->type << endl;
	ofstream* out = this->createClassFile(classname);

	this->generatePackage(out);

	// TODO: optional implements DelegateTrait 
	// TODO: factor trait name
	*out << "class " << classname << " implements DelegateTrait {" << endl;
	for (MyParser::DslDefList::const_iterator it = spec->defs->begin(); it != spec->defs->end(); it++) {
		this->generateDefinition(out, *it);
	}
	*out << "}" << endl;

	out->close();
}
