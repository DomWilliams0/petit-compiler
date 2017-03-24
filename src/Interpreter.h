/*
 * Interpreter.h
 *
 *  Created on: 24 mars 2017
 *      Author: mfallouh
 */

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "Element.h"

class Interpreter {
protected:
	Document doc;
public:
	Interpreter(Document d): doc(d){};
	virtual ~Interpreter();
	void solveScopes();

};

#endif /* INTERPRETER_H_ */
