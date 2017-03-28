#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <vector>
#include "Element.h"

enum UnaryOperator { EXCLAMATION, NEG, POS };
enum BinaryOperator { PLUS, MINUS, MULT, DIV, MODULO, LT, LE, GT, GE, EQ, NE, AND, OR, COMMA };

//==========================================================
class Expression : public Node
{
	public:
		virtual ~Expression() {}
		virtual void print(GraphPrinter *) const = 0;
		virtual std::string printSelf() const = 0;
		virtual ElementType getType() const = 0;
};

//==========================================================
class Variable : public Expression
{
	protected:
		std::string name;
		bool isLvalue;
		Expression *index;

	public:
		Variable(const std::string &name, Expression *index = nullptr, bool left = true) : name(name), isLvalue(left), index(index) {}
		~Variable();
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		ElementType getType() const { return VAR; }
};

//==========================================================
class ConstInteger : public Expression
{
	public:
		ConstInteger(uint64_t value) : value(value) {}
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		ElementType getType() const { return CONSTINT; }

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
		ElementType getType() const { return CONSTCHAR; }

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
		~Affectation();
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		ElementType getType() const { return AFFECTATION; }
};

enum IncrementType { POST_INC, POST_DEC, PRE_INC, PRE_DEC };

class AffectationIncrement : public Expression
{
	protected:
		IncrementType op;
		Variable *lvalue;

	public:
		AffectationIncrement(IncrementType op, Variable *lvalue) : op(op), lvalue(lvalue) {}
		~AffectationIncrement();

		void print(GraphPrinter *) const;
		std::string printSelf() const;
		ElementType getType() const { return AFFECTATION_INC; }
};

class AffectationCompound : public Expression
{
	protected:
		Variable *lvalue;
		BinaryOperator op;
		Expression *rvalue;

	public:
		AffectationCompound(Variable *lvalue, BinaryOperator op, Expression *rvalue) : lvalue(lvalue), op(op), rvalue(rvalue) {}
		~AffectationCompound();

		void print(GraphPrinter *) const;
		std::string printSelf() const;
		ElementType getType() const { return AFFECTATION_COMPOUND; }
};

//==========================================================
class FunctionAppel : public Expression
{
	protected:
		std::string funcName;
		std::vector<Expression *> args;

	public:
		FunctionAppel(const std::string &name, Expression *args = nullptr);
		~FunctionAppel();
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		ElementType getType() const { return FUNCAPPEL; }
};

//==========================================================
class UnaryExpression : public Expression
{
	protected:
		UnaryOperator op;
		Expression *expression;
	public:

		UnaryExpression(UnaryOperator op, Expression *expression) : op(op), expression(expression) {};
		~UnaryExpression();
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		ElementType getType() const  {return UNARY; }
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
		~BinaryExpression();
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		ElementType getType() const { return BINARY; }

		Expression *getLeftExpression() const { return lExpression; }
		Expression *getRightExpression() const { return rExpression; }
		BinaryOperator getOperator() const { return op; }
};

std::string binaryOpToString(BinaryOperator op);
std::string unaryOpToString(UnaryOperator op);

#endif
