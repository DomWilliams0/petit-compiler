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

class NullStatement : public Statement
{
	void print() const;
};

class Block : public Statement
{
	public:
		Block() {}
		Block(std::vector<Node *> *contents) : contents(contents) {}
		~Block();
		void print() const;

	protected:
		std::vector<Node *> *contents;
};

class Cond : public Statement
{
	public:
		void print() const;
		Cond(Block *iBlock, Expression *cond, Block *eBlock = nullptr) : ifBlock(iBlock), condition(cond), elseBlock(eBlock) {}
		~Cond();

		// only change if currently null
		void updateElse(Block *newElse);

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

class Return : public Statement
{
	public:
		void print() const;
		Return(Expression *value = nullptr) : value(value) {}

	protected:
		Expression *value;
};

#endif
