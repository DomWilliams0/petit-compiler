#pragma once
#include <string>
#include <list>

using namespace std;

enum Type { INT32, INT64, CHAR, VOID };
enum Unary_Operator{EXCLAMATION, NEG};
enum Binary_Operator { PLUS, MINUS, MULT, DIV, MODULO};
struct Value {
	Type type;
	union{
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
class Variable : Expression {
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
class Const : Expression {
protected:
	Value value;
public:
	Const(Value val) : value(val) {};
	virtual ~Const();
	void print();
};

//==========================================================
class Affectation : Expression {
protected:
	Variable lOperand;
	Expression * rOperand;

public :
	Affectation(Variable lOperand, Expression *rOperand) : lOperand(lOperand), rOperand(rOperand) {};
	virtual ~Affectation();
	void print();
};

//==========================================================
class Function_Appel : Expression {
protected:
	list<Expression*> args;
	string funcName;
public:
	Function_Appel(list<Expression *> args, string name) : args(args), funcName(name) {};
	virtual ~Function_Appel();
	void print();
};

//==========================================================
class Unary_Expression : Expression {
protected:
	Unary_Operator op;
	Expression * expression;
public:

	Unary_Expression(Unary_Operator op, Expression * expression) : op(op), expression(expression) {};
	virtual ~Unary_Expression();
	void print();
};

//==========================================================
class Binary_Expression : Expression {
protected:
	Binary_Operator op;
	Expression * lExpression;
	Expression * rExpression;
	
public:

	Binary_Expression(Expression * lexpression, Binary_Operator op, Expression * rexpression) : op(op), lExpression(lexpression), rExpression(rexpression) {};
	virtual ~Binary_Expression();
	void print();
};