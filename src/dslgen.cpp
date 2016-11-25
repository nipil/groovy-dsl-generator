#include <cstdlib>
#include <ctime>

#include <locale>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "myparser.h"
#include "dslgen.h"

const string DslGen::BASE_OUTPUT = "output";
const string DslGen::CLASS_PATH = "src/main/groovy";
const string DslGen::SAMPLE_DSL = "sample.dsl";
const string DslGen::DELEGATE_CLASS_NAME = "DelegateTrait";
const string DslGen::MASTER_SCRIPT_TYPE = "masterScript";

DslGen::DslGen(const MyParser& myparser)
: parser(myparser) {
	// set random seed, based on current unix time
	srandom(time(NULL));
}

void DslGen::generate() {
	this->packagePath = parser.getPackage();
	replace(this->packagePath.begin(), this->packagePath.end(), '.', '/');
	this->createOutputDirectory();
	this->generateSpecifications();
	this->generateMasterSpec();
	this->generateDelegate();
	this->generateExample();
	this->generateCommandHint();
}

void DslGen::generateMasterSpec() {
	MyParser::SpecDef master;
	master.type = (string *) &this->MASTER_SCRIPT_TYPE;
	master.defs = new MyParser::DslDefList();
	MyParser::Definitions pd = parser.getDefinitions();
	for (MyParser::Definitions::const_iterator it = pd.begin(); it != pd.end(); it++) {
		master.defs->push_back(it->second);
	}
	this->scanSpecForMembers(&master); // prevents const on method
	this->generateSpecification(&master, true);
}

void DslGen::generateDelegate() const {
	cout << "Generating class " << this->DELEGATE_CLASS_NAME << " for closure delegation" << endl;
	ofstream* out = this->createClassFile(this->DELEGATE_CLASS_NAME);

	this->generatePackage(out);

	*out
		<< "import groovy.transform.TypeChecked" << endl
		<< endl
		<< "@TypeChecked" << endl
		<<"trait DelegateTrait {" << endl
		<< endl
		<< "\tdef delegate(Closure cl, Object to) {" << endl
		<< "\t\tdef code = cl.rehydrate(to, this, this)" << endl
		<< "\t\tcode.resolveStrategy = Closure.DELEGATE_ONLY" << endl
		<< "\t\tcode()" << endl
		<< "\t}" << endl
		<< "}" << endl;

	out->close();
}

void DslGen::createOutputDirectory() const {
	string rp = this->BASE_OUTPUT + "/" + this->CLASS_PATH + "/" + this->packagePath;
	cout << "Creating folder " << rp << endl;
	int result = system(string("mkdir -p " + rp).c_str());
	if (result != 0) {
		cout << "error while creating directory" << endl;
		exit(1);
	}
}

void DslGen::generateSpecifications() {
	const MyParser::Specifications& specs = parser.getSpecifications();
	for (MyParser::Specifications::const_iterator it = specs.begin(); it != specs.end(); it++) {
		this->scanSpecForMembers(it->second);
		this->generateSpecification(it->second);
	}
}

ofstream* DslGen::createClassFile(const string& classname) const {
	string filepath = this->CLASS_PATH + "/" + this->packagePath + "/" + classname + ".groovy";
	return this->createOutFile(filepath);
}

ofstream* DslGen::createOutFile(const string& filepath) const {
	string rp = this->BASE_OUTPUT + "/" + filepath;
	ofstream* f = new ofstream(rp.c_str(), ofstream::trunc);
	if (!f->is_open()) {
		cerr << "cannot open output file " << rp << endl;
		exit(1);
	}
	return f;
}

void DslGen::generatePackage(ostream* out) const {
	*out << "package " << parser.getPackage() << endl << endl;
}

string DslGen::getClassName(string customTypeName) const {
	string classname = customTypeName + "Spec";
	locale loc;
	classname[0] = toupper(classname[0], loc);
	return classname;
}

void DslGen::generateDefinition(ostream* out, MyParser::DslDef* def) const {
	int n;
	*out << "\tvoid " << *def->keyword << "(";

	// method signature
	n = 0;
	for (MyParser::TypeList::const_iterator it = def->types->begin(); it != def->types->end(); it++) {
		n += 1;
		if (it != def->types->begin()) {
			*out << ", ";
		}
		string& type = **it;
		if (parser.hasCustomType(type)) {
			// see @DelegatesTo from GroovyDoc
			// http://docs.groovy-lang.org/docs/latest/html/documentation/core-domain-specific-languages.html#TheDelegatesToannotation-DelegatesTo
			*out << "@DelegatesTo(strategy=Closure.DELEGATE_ONLY, value="
				<< this->getClassName(type) << ") "
				<< "Closure cl" << n;
		} else if (type == "num") {
			*out << "int num" << n;
		} else if (type == "txt") {
			*out << "String txt" << n;
		} else {
			cerr << "unhandled standard type " << type << endl;
			exit(1);
		}
	}

	*out << ") {" << endl;

	// method body
	n = 0;
	for (MyParser::TypeList::const_iterator it = def->types->begin(); it != def->types->end(); it++) {
		n += 1;
		string& type = **it;
		*out << "\t\t";
		if (parser.hasCustomType(type)) {
			*out << "this.delegate(cl" << n << ", " << type << ")" << endl;
		} else {
			*out << "println \"" << *def->keyword << ": " << type << n << "=${" << type << n << "}\"" << endl;
		}
	}

	*out << "\t}" << endl << endl;
}

void DslGen::scanSpecForMembers(const MyParser::SpecDef* const spec) {
	// clear used types
	this->usedCustomTypes.clear();

	// append custom types to used types
	for (MyParser::DslDefList::const_iterator it_defs = spec->defs->begin(); it_defs != spec->defs->end(); it_defs++) {
		MyParser::DslDef* def = *it_defs;
		for (MyParser::TypeList::const_iterator it = def->types->begin(); it != def->types->end(); it++) {
			string type = **it;
			if (parser.hasCustomType(type)) {
				this->usedCustomTypes.insert(**it);
			}
		}
	}
}

void DslGen::generateSpecification(const MyParser::SpecDef* const spec, bool master) const {
	string classname = this->getClassName(*spec->type);
	cout << "Generating class " << classname << " for spec " << *spec->type << endl;
	ofstream* out = this->createClassFile(classname);

	this->generatePackage(out);

	*out << "import groovy.transform.TypeChecked" << endl << endl << "@TypeChecked" << endl;

	// generate class declaration
	if (master) {
		*out << "abstract ";
	}
	*out << "class " << classname << " ";
	if (master) {
		*out << "extends Script ";
	}
	if (usedCustomTypes.size() > 0) {
		*out << "implements " << this->DELEGATE_CLASS_NAME << " ";
	}
	*out << "{" << endl << endl;
	for (MyParser::DslDefList::const_iterator it = spec->defs->begin(); it != spec->defs->end(); it++) {
		this->generateDefinition(out, *it);
	}

	// list used types as members
	for (set<string>::const_iterator it = usedCustomTypes.begin(); it != usedCustomTypes.end(); it++) {
		*out << "\t" << this->getClassName(*it) << " " << *it << " = new " << this->getClassName(*it) << "()" << endl;
	}

	*out << "}" << endl;

	out->close();
}

void DslGen::generateExample() const {
	cout << "Generating sample dsl file " << this->SAMPLE_DSL << endl;
	ofstream* out = this->createOutFile(this->SAMPLE_DSL);

	MyParser::Definitions pd = parser.getDefinitions();
	for (MyParser::Definitions::const_iterator it = pd.begin(); it != pd.end(); it++) {
		this->generateExampleDefinition(out, it->second, 0);
	}

	out->close();
}

void DslGen::generateExampleDefinition(ostream* out, const MyParser::DslDef* const def, int level) const {

	for (int i= 0; i<level; i++) {
		*out << "\t";
	}

	*out << *def->keyword << " ";

	for (MyParser::TypeList::const_iterator it = def->types->begin(); it != def->types->end(); it++) {
		if (it != def->types->begin()) {
			*out << ", ";
		}
		string& type = **it;
		if (parser.hasCustomType(type)) {
			this->generateExampleDefinitionFromSpec(out, type, level+1);
		} else if (type == "num") {
			*out << random() % 1000;
		} else if (type == "txt") {
			int n = random() % 15 + 5;
			*out << "\"";
			for (int i=0; i<n; i++) {
				*out << (char) ('a' + random() % 26);
			}
			*out << "\"";
		} else {
			cerr << "unhandled standard type " << type << endl;
			exit(1);
		}
	}
	*out << endl;
}

void DslGen::generateExampleDefinitionFromSpec(ostream* out, const string& customType, int level) const {

	const MyParser::Specifications& specs = parser.getSpecifications();

	MyParser::Specifications::const_iterator it_spec = specs.find(customType);
	if (it_spec == specs.end()) {
		cerr << "could not find spec to generate sample dsl" << endl;
		exit(1);
	}
	const MyParser::SpecDef* spec = it_spec->second;

	*out << "{" << endl;

	for (MyParser::DslDefList::const_iterator it = spec->defs->begin(); it != spec->defs->end(); it++) {
		this->generateExampleDefinition(out, *it, level);
	}

	for (int i= 0; i<level-1; i++) {
		*out << "\t";
	}
	*out << "}";
}

void DslGen::generateCommandHint() const {
	cout << "To parse DSL: groovy -cp "
		<< this->BASE_OUTPUT << "/" << this->CLASS_PATH << " "
		<< "-b " << parser.getPackage() << "." << this->getClassName(this->MASTER_SCRIPT_TYPE) << " "
		<< this->BASE_OUTPUT << "/" << this->SAMPLE_DSL << endl;
}
