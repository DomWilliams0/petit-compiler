/*
 * Interpreter.cpp
 *
 *  Created on: 24 mars 2017
 *      Author: mfallouh
 */

#include "Interpreter.h"


Interpreter::~Interpreter() {
	// TODO Auto-generated destructor stub
}

void Interpreter::solveScopes(){
	varCounter = 0;
	environments = new std::stack<SymbolTable*>;
	std::vector<Element *> elements=doc.getElements();
	SymbolTable *s = new SymbolTable();

	environments->push(s);

	for(uint i = 0; i<elements.size(); i++)
	{
		if(elements[i]->getType() == FUNC_DEF)
		{
			FuncDef *f = (FuncDef*)(elements[i]);
			(s->funct)[f->getIdentifier()]=f;
			f->solveScopes(environments);
		}
		else if(elements[i]->getType() == VAR_DECL)
		{
			VarDecl* decl = (VarDecl*)(elements[i]);
			(s->vars)[decl->getIdentifier()] = {decl, varCounter++};
		}
		else if(elements[i]->getType() == VAR_DEF)
		{
			VarDef* def = (VarDef*)(elements[i]);
			(s->vars)[def->getIdentifier()] = {def, varCounter++};
		}
	}
}

