#include <iostream>
#include "expression.h"

void Expression::print()
{
}

Variable::~Variable()
{
}

void Variable::print()
{
	std::cout << name;
	if (index != NULL)
	{
		std::cout << "[";
		index->print();
		std::cout << "]";
	}
}

Const::~Const()
{
}

void Const::print()
{
	switch (value.type) {
		case INT32:
			std::cout << value.i32;
			break;
		case INT64:
			std::cout << value.i64;
			break;
		case CHAR:
			std::cout << value.c;
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
	std::cout << " = ";
	rOperand->print();
}

FunctionAppel::~FunctionAppel()
{
	for (int i = 0; i < args.size;i++) {
		delete(args.front);
		args.popfront;
	}
}

void FunctionAppel::print()
{
	std::cout << funcName << "( ";
	std::list<Expression*>::constiterator iterator;
	for (iterator = args.begin(); iterator != args.end(); ++iterator) {
		(*iterator)->print();
	}
	std::cout << ")";
}

UnaryExpression::~UnaryExpression()
{
	delete expression;
}

void UnaryExpression::print()
{
	switch (op) {
		case NEG:
			std::cout << "-";
			break;
		case EXCLAMATION:
			std::cout << "!";
			break;
	}
	expression->print();
}

BinaryExpression::~BinaryExpression()
{
	delete lExpression;
	delete rExpression;
}

void BinaryExpression::print()
{
	lExpression->print();
	switch (op) {
		case PLUS:
			std::cout << "+";
			break;
		case MINUS:
			std::cout << "-";
			break;
		case MULT:
			std::cout << "*";
			break;
		case DIV:
			std::cout << "/";
			break;
		case MODULO:
			std::cout << "%";
			break;
	}
	rExpression->print();
}
