/*
 * Interpreter.cpp
 *
 *  Created on: 24 mars 2017
 *      Author: mfallouh
 */

#include "Interpreter.h"


Interpreter::~Interpreter() {
}

void Interpreter::solveScopes(){
	varCounter = 0;
	environments = new std::deque<SymbolTable*>;
	std::vector<Element *> elements=doc->getElements();
	SymbolTable *s = new SymbolTable();

	environments->push_back(s);

	for(size_t i = 0; i<elements.size(); i++)
	{
		elements[i]->solveScopes(environments, &varCounter);
	}
	delete environments->back();
	environments->pop_back();
}

