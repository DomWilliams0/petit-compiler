#ifndef STATEMENT_H
#define STATEMENT_H

#include <vector>
#include "Element.h"
#include "Expression.h"


class Statement : public Node
{
	public:
		virtual ~Statement() {}
		virtual int getType() const = 0;
		virtual void print(GraphPrinter *) const = 0;
		virtual std::string printSelf() const = 0;

};

class Block : public Statement
{
	public:
		Block() {}
		Block(std::vector<Node *> *contents) : contents(contents) {}
		~Block() {}
		void createBlocks();
		int getType() const {
			return BLOCK;
		}
		std::vector<Node*>*& getContents() {
			return contents;
		}

		void setContents(std::vector<Node*>*& contents) {
			this->contents = contents;
		}
		void print(GraphPrinter *) const;
		std::string printSelf() const;

		;
	protected:
		std::vector<Node *> *contents;
};

class Cond : public Statement
{
	public:
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		Cond(Statement *iBlock, Expression *cond, Statement *eBlock = nullptr) : ifBlock(iBlock), condition(cond), elseBlock(eBlock) {}
		~Cond() {}

		// only change if currently null
		void updateElse(Statement *newElse);
	int getType() const {
		return COND;
	}
	 Statement*& getElseBlock()  {
		return elseBlock;
	}

	 Statement*& getIfBlock()  {
		return ifBlock;
	}

	;
	protected:
		Statement *ifBlock;
		Expression *condition;
		Statement *elseBlock;
};

class Iter : public Statement
{
	public:
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		Iter(Statement *iBlock, Expression *cond) : iterBlock(iBlock), condition(cond) {}
		~Iter() {}
	int getType() const {
		return ITER;
	}
	 Statement*& getIterBlock()  {
		return iterBlock;
	}

	;
	protected:
		Statement *iterBlock;
		Expression *condition;
};

class Return : public Statement
{
	public:
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		int getType() const { return RETURN_STAT; }
		Return(Expression *value = nullptr) : value(value) {}
		~Return() {}

	protected:
		Expression *value;
};

#endif
