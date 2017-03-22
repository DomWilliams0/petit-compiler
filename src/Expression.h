#pragma once
#include <string>
#include <list>

enum Type { INT32, INT64, CHAR, VOID };
enum UnaryOperator{ EXCLAMATION, NEG };
enum BinaryOperator { PLUS, MINUS, MULT, DIV, MODULO };

struct Value {
	Type type;
	union {
		int32_t i32;
		int64_t i64;
		char c;
	};
};

//==========================================================
class Expression {
	public:
		virtual void print() = 0;
};

//==========================================================
class Variable : public Expression {
	protected:
		bool isLvalue;
		Expression *index;
		string name;
	public:
		Variable(string name, Expression *expr = NULL, bool left = true) : name(name), isLvalue(left), index(expr) {};
		virtual ~Variable();
		void print();
};

//==========================================================
class Const : public Expression {
	protected:
		Value value;
	public:
		Const(Value val) : value(val) {};
		virtual ~Const();
		void print();
};

//==========================================================
class Affectation : public Expression {
	protected:
		Variable lOperand;
		Expression * rOperand;

	public :
		Affectation(Variable lOperand, Expression *rOperand) : lOperand(lOperand), rOperand(rOperand) {};
		virtual ~Affectation();
		void print();
};

//==========================================================
class FunctionAppel : public Expression {
	protected:
		list<Expression*> args;
		string funcName;
	public:
		FunctionAppel(list<Expression *> args, string name) : args(args), funcName(name) {};
		virtual ~FunctionAppel();
		void print();
};

//==========================================================
class UnaryExpression : public Expression {
	protected:
		UnaryOperator op;
		Expression * expression;
	public:

		UnaryExpression(UnaryOperator op, Expression * expression) : op(op), expression(expression) {};
		virtual ~UnaryExpression();
		void print();
};

//==========================================================
class BinaryExpression : public Expression {
	protected:
		BinaryOperator op;
		Expression * lExpression;
		Expression * rExpression;

	public:

		BinaryExpression(Expression * lexpression, BinaryOperator op, Expression * rexpression) : op(op), lExpression(lexpression), rExpression(rexpression) {};
		virtual ~BinaryExpression();
		void print();
};
