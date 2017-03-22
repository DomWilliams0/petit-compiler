#include "expression.h"
#include <iostream>

void Expression::print()
{
}

Variable::~Variable()
{
}

void Variable::print()
{
	cout << name;
	if (index != NULL)
	{
		cout << "[";
		index->print();
		cout << "]";
	}
}

Const::~Const()
{
}

void Const::print()
{
	switch (value.type) {
	case INT32:
		cout << value.i32;
		break;
	case INT64:
		cout << value.i64;
		break;
	case CHAR:
		cout << value.c;
		break;
	}
	
}

Affectation::~Affectation()
{
	delete(rOperand);
}

void Affectation::print()
{
	lOperand.print();
	cout << " = ";
	rOperand->print();
}

Function_Appel::~Function_Appel()
{
	for (int i = 0; i < args.size;i++) {
		delete(args.front);
		args.pop_front;
	}
}

void Function_Appel::print()
{
	cout << funcName << "( ";
	std::list<Expression*>::const_iterator iterator;
	for (iterator = args.begin(); iterator != args.end(); ++iterator) {
		(*iterator)->print();
	}
	cout << ")";
}

Unary_Expression::~Unary_Expression()
{
	delete expression;
}

void Unary_Expression::print()
{
	switch (op) {
	case NEG:
		cout << "-";
		break;
	case EXCLAMATION:
		cout << "!";
		break;
	}
	expression->print();
}

Binary_Expression::~Binary_Expression()
{
	delete lExpression;
	delete rExpression;
}

void Binary_Expression::print()
{
	lExpression->print();
	switch (op) {
	case PLUS:
		cout << "+";
		break;
	case MINUS:
		cout << "-";
		break;
	case MULT:
		cout << "*";
		break;
	case DIV:
		cout << "/";
		break;
	case MODULO:
		cout << "%";
		break;
	}
	rExpression->print();
}
