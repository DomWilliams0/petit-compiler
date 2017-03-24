#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <vector>
#include "Element.h"

enum UnaryOperator { EXCLAMATION, NEG, POS };
enum BinaryOperator { PLUS, MINUS, MULT, DIV, MODULO, LT, LE, GT, GE, EQ, NE, AND, OR, COMMA };
enum ExprTypes {VAR, CONSTINT, CONSTCHAR, AFFECTATION, FUNCAPPEL, UNARY, BINARY};

//==========================================================
class Expression : public Node
{
	public:
		virtual ~Expression() {}
		virtual void print(GraphPrinter *) const = 0;
		virtual std::string printSelf() const = 0;
		virtual int getType() const = 0;
};

//==========================================================
class Variable : public Expression
{
	protected:
		std::string *name;
		bool isLvalue;
		Expression *index;

	public:
		Variable(std::string *name, Expression *index = nullptr, bool left = true) : name(name), isLvalue(left), index(index) {}
		virtual ~Variable();
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		int getType() const { return VAR; }
};

//==========================================================
class ConstInteger : public Expression
{
	public:
		ConstInteger(uint64_t value) : value(value) {}
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		int getType() const { return CONSTINT; }

	protected:
		uint64_t value;
};

//==========================================================
class ConstCharacter : public Expression
{
	public:
		ConstCharacter(char value) : value(value) {}
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		int getType() const { return CONSTCHAR; }

	protected:
		char value;
};

//==========================================================
class Affectation : public Expression
{
	protected:
		Variable *lOperand;
		Expression *rOperand;

	public :
		Affectation(Variable *lOperand, Expression *rOperand) : lOperand(lOperand), rOperand(rOperand) {};
		virtual ~Affectation();
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		int getType() const { return AFFECTATION; }
};

enum IncrementType { POST_INC, POST_DEC, PRE_INC, PRE_DEC };

Expression *newAffectationIncrement(Variable *lvalue, IncrementType type);

//==========================================================
class FunctionAppel : public Expression
{
	protected:
		std::string *funcName;
		std::vector<Expression *> args;

	public:
		FunctionAppel(std::string *name, Expression *args = nullptr);
		virtual ~FunctionAppel();
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		int getType() const { return FUNCAPPEL; }
};

//==========================================================
class UnaryExpression : public Expression
{
	protected:
		UnaryOperator op;
		Expression *expression;
	public:

		UnaryExpression(UnaryOperator op, Expression *expression) : op(op), expression(expression) {};
		virtual ~UnaryExpression();
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		int getType() const  {return UNARY; }
};

//==========================================================
class BinaryExpression : public Expression
{
	protected:
		BinaryOperator op;
		Expression *lExpression;
		Expression *rExpression;

	public:

		BinaryExpression(Expression *lexpression, BinaryOperator op, Expression *rexpression) : op(op), lExpression(lexpression), rExpression(rexpression) {};
		virtual ~BinaryExpression();
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		int getType() const { return BINARY; }

		Expression *getLeftExpression() const { return lExpression; }
		Expression *getRightExpression() const { return rExpression; }
		BinaryOperator getOperator() const { return op; }
};

std::string binaryOpToString(BinaryOperator op);
std::string unaryOpToString(UnaryOperator op);

#endif
