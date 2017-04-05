#ifndef STATEMENT_H
#define STATEMENT_H

#include <vector>
#include "Element.h"
#include "Expression.h"
#include "Interpreter.h"
#include <map>


class Statement : public Node
{
	public:
		virtual ~Statement() {}
		virtual ElementType getType() const = 0;
		virtual void print(GraphPrinter *) const = 0;
		virtual std::string printSelf() const = 0;

		virtual Type solveScopes(std::deque<SymbolTable*>*, int *varCounter, CFG *cfg, ErrorList &errors) = 0;
		virtual std::string buildIR(CFG* cfg) = 0;
};

class Block : public Statement
{
	public:
		Block() : contents(nullptr){}
		Block(std::vector<Node *> *contents) : contents(contents) {}
		~Block();
		void createBlocks();
		ElementType getType() const {
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

		Type solveScopes(std::deque<SymbolTable*>*, int *varCounter, CFG *cfg, ErrorList &errors);
		std::string buildIR(CFG* cfg);
		
	protected:
		std::vector<Node *> *contents;
};

class Cond : public Statement
{
	public:
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		Cond(Statement *iBlock, Expression *cond, Statement *eBlock = nullptr) : ifBlock(iBlock), condition(cond), elseBlock(eBlock) {}
		~Cond();

		// only change if currently null
		void updateElse(Statement *newElse);

		Type solveScopes(std::deque<SymbolTable*>*, int *varCounter, CFG *cfg, ErrorList &errors);
		std::string buildIR(CFG* cfg);

		ElementType getType() const {
			return COND;
		}
		 Statement*& getElseBlock()  {
			return elseBlock;
		}

		 Statement*& getIfBlock()  {
			return ifBlock;
		}

	protected:
		Statement *ifBlock;
		Expression *condition;
		Statement *elseBlock;
};

class For : public Statement
{
	public:
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		For(Node *init, Expression *cond, Expression *inc, Statement *block) : init(init), cond(cond), inc(inc), block(block) {}
		~For();

		ElementType getType() const { return FOR_ITER; }

		Statement*& getBlock()  {
			return block;
		}
		
		Type solveScopes(std::deque<SymbolTable*>*, int *varCounter, CFG *cfg, ErrorList &errors);
		std::string buildIR(CFG* cfg);

	protected:
		Node *init;
		Expression *cond;
		Expression *inc;
		Statement *block;
};

class Iter : public Statement
{
	public:
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		Iter(Expression *cond, Statement *block) : condition(cond), iterBlock(block) {}
		~Iter();
		ElementType getType() const {
			return ITER;
		}
		Statement*& getIterBlock()  {
			return iterBlock;
		}
		
		Type solveScopes(std::deque<SymbolTable*>*, int *varCounter, CFG *cfg, ErrorList &errors);
		std::string buildIR(CFG* cfg);

	;
	protected:
		Expression *condition;
		Statement *iterBlock;
};

class Return : public Statement
{
	public:
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		ElementType getType() const { return RETURN_STAT; }
		Return(Expression *value = nullptr) : value(value) {}
		~Return();

		Type solveScopes(std::deque<SymbolTable*>*, int *varCounter, CFG *cfg, ErrorList &errors);
		std::string buildIR(CFG* cfg);

	protected:
		Expression *value;
};

#endif
