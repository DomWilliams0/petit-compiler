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

typedef struct VarRef
{
	Node* ref;
	int id;
} VarRef;

typedef struct SymbolTable
{
	std::map<std::string,VarRef> vars;
	std::map<std::string, Element*> funct;
} SymbolTable;

class Interpreter {
protected:
	Document* doc;
	std::deque<SymbolTable*>* environments;
	int varCounter;
	std::vector<CFG*> CFGs;
public:
	Interpreter(Document* d): doc(d), varCounter(0){};
	virtual ~Interpreter();
	void solveScopes();
	void buildIR();

};

#endif /* INTERPRETER_H_ */
