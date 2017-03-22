/*
 * Statement.h
 *
 *  Created on: 22 mars 2017
 *      Author: lboucaud
 */

#ifndef STATEMENT_H_
#define STATEMENT_H_
#include "Element.h"
#include "expression.h"
#include <vector>

class Statement {
public:
	virtual ~Statement();
	void print()=0;
protected:
	Statement();
};


class Block : public Statement
{
public:
	Block();
	virtual ~Block();
	void print();
protected:
	std::vector<VarDecl> declarations;
	std::vector<VarDef> definitions;
	std::vector<Statement> statements;
};

class Cond : public Statement
{
public:
	void print();
	Cond(Block iBlock,Expression cond):ifBlock(iBlock),condition(cond),elseBlock(NULL){};
	Cond(Block iBlock,Block eBlock,Expression cond):ifBlock(iBlock),elseBlock(eBlock),condition(cond){};
	virtual ~Cond();
protected:
	Block ifBlock;
	Block elseBlock;
	Expression condition;
};

class Iter : public Statement
{
public:
	void print();
	Iter(Block iBlock,Expression cond):iterBlock(iBlock),condition(cond){};
	virtual ~Iter();
protected:
	Block iterBlock;
	Expression condition;
};



#endif /* STATEMENT_H_ */
