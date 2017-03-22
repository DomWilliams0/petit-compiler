#ifndef STATEMENT_H
#define STATEMENT_H

#include "Element.h"
#include "Expression.h"
#include <vector>

class Statement {
public:
	virtual ~Statement() {}
	virtual void print() = 0;

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
		Cond(Block *iBlock,Expression *cond):ifBlock(iBlock),condition(cond),elseBlock(NULL){};
		Cond(Block *iBlock,Block *eBlock,Expression *cond):ifBlock(iBlock),elseBlock(eBlock),condition(cond){};
		~Cond();
	protected:
		Block *ifBlock;
		Block *elseBlock;
		Expression *condition;
};

class Iter : public Statement
{
	public:
		void print();
		Iter(Block *iBlock,Expression *cond):iterBlock(iBlock),condition(cond){};
		~Iter();
	protected:
		Block *iterBlock;
		Expression *condition;
};

#endif
