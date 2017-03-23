#ifndef STATEMENT_H
#define STATEMENT_H

#include <vector>
#include "Element.h"
#include "Expression.h"

class Statement : public Node
{
	public:
		virtual ~Statement() {}
		virtual void print() const = 0;

	protected:
};

class Block : public Statement
{
	public:
		Block() {}
		Block(std::vector<Node *> *contents) : contents(contents) {}
		~Block() {}
		void print() const;

	protected:
		std::vector<Node *> *contents;
};

class Cond : public Statement
{
	public:
		void print() const;
		Cond(Statement *iBlock, Expression *cond, Statement *eBlock = nullptr) : ifBlock(iBlock), condition(cond), elseBlock(eBlock) {}
		~Cond() {}

		// only change if currently null
		void updateElse(Statement *newElse);

	protected:
		Statement *ifBlock;
		Expression *condition;
		Statement *elseBlock;
};

class Iter : public Statement
{
	public:
		void print() const;
		Iter(Statement *iBlock, Expression *cond) : iterBlock(iBlock), condition(cond) {}
		~Iter() {}
	protected:
		Statement *iterBlock;
		Expression *condition;
};

class Return : public Statement
{
	public:
		void print() const;
		Return(Expression *value = nullptr) : value(value) {}
		~Return() {}

	protected:
		Expression *value;
};

#endif
