/*
 * Interpreter.h
 *
 *  Created on: 24 mars 2017
 *      Author: mfallouh
 */

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "Element.h"
#include <stack>
#include <map>

typedef struct VarRef
{
	Node* ref;
	int id;
} VarRef;

typedef struct SymbolTable
{
	std::map<std::string,VarRef> vars;
	std::map<std::string,FuncDef*> funct;
} SymbolTable;

class Interpreter {
protected:
	Document doc;
	std::stack<SymbolTable*>* environments;
	int varCounter;
public:
	Interpreter(Document d): doc(d), varCounter(0){};
	virtual ~Interpreter();
	void solveScopes();

};

#endif /* INTERPRETER_H_ */
