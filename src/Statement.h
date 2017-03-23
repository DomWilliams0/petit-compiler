#ifndef STATEMENT_H
#define STATEMENT_H

#include <vector>
#include "Element.h"
#include "Expression.h"

class Statement : public Node
{
	public:
		virtual ~Statement();
		virtual void print() const = 0;

	protected:
		Statement();
};


class Block : public Statement
{
	public:
		~Block();
		void print() const;

	protected:
		std::vector<VarDecl *> declarations;
		std::vector<VarDef *> definitions;
		std::vector<Statement *> statements;
};

class Cond : public Statement
{
	public:
		void print() const;
		Cond(Block *iBlock, Expression *cond, Block *eBlock = nullptr) : ifBlock(iBlock), condition(cond), elseBlock(eBlock) {}
		~Cond();
	protected:
		Block *ifBlock;
		Expression *condition;
		Block *elseBlock;
};

class Iter : public Statement
{
	public:
		void print() const;
		Iter(Block *iBlock, Expression *cond) : iterBlock(iBlock), condition(cond) {}
		~Iter();
	protected:
		Block *iterBlock;
		Expression *condition;
};

#endif
