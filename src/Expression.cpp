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
	std::cout << *name;
	if (index != nullptr)
	{
		std::cout << "[";
		index->print();
		std::cout << "]";
	}
}

void ConstInteger::print() const
{
	std::cout << value;
}

void ConstCharacter::print() const
{
	std::cout << "'" << value << "'";
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

Expression *newAffectationIncrement(Variable *lvalue, IncrementType type)
{
	// TODO actually obey the increment type, this is just a placeholder
	BinaryExpression *inc = new BinaryExpression(lvalue, PLUS, new ConstInteger(1));
	return new Affectation(lvalue, inc);
}

FunctionAppel::~FunctionAppel()
{
	args->print();
}

void FunctionAppel::print() const
{
	std::cout << *funcName << "( ";
	args->print();
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
		case POS:
			std::cout << "+";
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
		case LT:
			std::cout << "<";
			break;
		case LE:
			std::cout << "<=";
			break;
		case GT:
			std::cout << ">";
			break;
		case GE:
			std::cout << ">=";
			break;
		case EQ:
			std::cout << "==";
			break;
		case NE:
			std::cout << "!=";
			break;
		case AND:
			std::cout << "&&";
			break;
		case OR:
			std::cout << "||";
			break;
		case COMMA:
			std::cout << ",";
			break;
	}

	rExpression->print();
}
