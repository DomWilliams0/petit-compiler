#include <iostream>
#include <algorithm>
#include "Expression.h"

void Expression::print() const
{
}

Variable::~Variable()
{
}

void Variable::print() const
{
	std::cout << name;
	if (index != nullptr)
	{
		std::cout << "[";
		index->print();
		std::cout << "]";
	}
}

Const::~Const()
{
}

void Const::print() const
{
	value.printValue();
}

Affectation::~Affectation()
{
	delete rOperand;
}

void Affectation::print() const
{
	lOperand->print();
	std::cout << " = ";
	rOperand->print();
}

FunctionAppel::~FunctionAppel()
{
	for (size_t i = 0; i < args.size(); i++)
	{
		delete args.back();
		args.pop_back();
	}
}

void FunctionAppel::print() const
{
	std::cout << funcName << "( ";
	std::for_each(args.begin(), args.end(), [](Expression *e) { e->print(); });
	std::cout << ")";
}

UnaryExpression::~UnaryExpression()
{
	delete expression;
}

void UnaryExpression::print() const
{
	switch (op)
	{
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

void BinaryExpression::print() const
{
	lExpression->print();

	switch (op)
	{
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
