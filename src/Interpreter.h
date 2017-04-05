/*
 * Interpreter.h
 *
 *  Created on: 24 mars 2017
 *      Author: mfallouh
 */

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

//#include "Element.h"
#include <stack>
#include <map>
#include <vector>

//#include "IR.h"
class Node;
class Element;
class Document;
class CFG;

struct VarRef
{
	Node* ref;
	int id;
};

struct SymbolTable
{
	std::map<std::string,VarRef> vars;
	std::map<std::string, Element*> funct;
};

struct Error
{
	Error(const std::string &msg) : msg(msg) {}

	std::string msg;
	// TODO further attributes about location
};

struct ErrorList
{
	std::vector<Error> errors;

	inline void addError(const std::string &err) { errors.emplace_back(err); }
};

class Interpreter {

protected:
	Document* doc;
	std::deque<SymbolTable*>* environments;
	int varCounter;
	std::vector<CFG*> CFGs;
public:
	Interpreter(Document* d): doc(d), varCounter(0){};
	virtual ~Interpreter();
	void solveScopes(ErrorList &errors);
	void buildIR();

};

#endif /* INTERPRETER_H_ */
