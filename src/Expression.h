#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <vector>
#include "Element.h"

enum UnaryOperator { EXCLAMATION, NEG };
enum BinaryOperator { PLUS, MINUS, MULT, DIV, MODULO };

//==========================================================
class Expression : public Node
{
	public:
		virtual ~Expression() {}
		virtual void print() const = 0;
};

//==========================================================
class Variable : public Expression
{
	protected:
		std::string name;
		bool isLvalue;
		Expression *index;

	public:
		Variable(const std::string &name, Expression *expr = NULL, bool left = true) : name(name), isLvalue(left), index(expr) {};
		virtual ~Variable();
		void print() const;
};

//==========================================================
class Const : public Expression
{
	protected:
		Value value;

	public:
		Const(const Value &val) : value(val) {};
		virtual ~Const();
		void print() const;
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
		void print() const;
};

//==========================================================
class FunctionAppel : public Expression
{
	protected:
		std::string funcName;
		std::vector<Expression *> args;

	public:
		FunctionAppel(const std::string &name, const std::initializer_list<Expression *> &args) : funcName(name), args(args) {};
		virtual ~FunctionAppel();
		void print() const;
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
		void print() const;
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
		void print() const;
};
#endif
