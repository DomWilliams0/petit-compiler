/*
 * Interpreter.cpp
 *
 *  Created on: 24 mars 2017
 *      Author: mfallouh
 */

#include "Interpreter.h"
#include "Element.h"
#include "IR.h"

Interpreter::~Interpreter() {
	// TODO Auto-generated destructor stub
}

void Interpreter::solveScopes(ErrorList &errors) {
	varCounter = 0;
	environments = new std::deque<SymbolTable*>;
	std::vector<Element *> elements=doc->getElements();
	SymbolTable *s = new SymbolTable();

	environments->push_back(s);
	std::vector<Element*> globalVars;

	CFG *curCFG = nullptr;

	for(int i = 0; i<elements.size(); i++)
	{
		int type = elements[i]->getType();
		if (type == VAR_DEF || type == VAR_DECL)
		{
			globalVars.push_back((VarDef*)elements[i]);
			curCFG = nullptr;
		}
		else if (type == FUNC_DEF)
		{
			CFGs.push_back(new CFG((FuncDef*)elements[i]));
			curCFG = CFGs.back();
			for (auto el : globalVars)
			{				
				if (el->getType() == VAR_DEF)
				{
					CFGs.back()->add_to_symbol_table(el->getIdentifier(), ((VarDef*)el)->getVarType());
				}
				else
				{
					CFGs.back()->add_to_symbol_table(el->getIdentifier(), ((VarDecl*)el)->getVarType());
				}
			}
		}

		elements[i]->solveScopes(environments, &varCounter, curCFG, errors);
	}
	delete environments->back();
	environments->pop_back();
}

void Interpreter::buildIR()
{

}

