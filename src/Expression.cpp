#include <algorithm>
#include <sstream>
#include "Printer.h"
#include "Expression.h"

Variable::~Variable()
{
}

std::string Variable::printSelf() const
{
	std::stringstream out;
	out << "Var " << *name;
	return out.str();
}

void Variable::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);

	if (index != nullptr)
	{
		printer->addConnection((Node *)this, index);
		index->print(printer);
	}
}

std::string ConstInteger::printSelf() const
{
	return std::to_string(value);
}

void ConstInteger::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);
}

std::string ConstCharacter::printSelf() const
{
	std::stringstream out;
	out << "'" << value << "'";
	return out.str();
}

void ConstCharacter::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);
}

Affectation::~Affectation()
{
	delete rOperand;
}

std::string Affectation::printSelf() const
{
	return "=";
}

void Affectation::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);

	printer->addConnection((Node *)this, lOperand);
	printer->addConnection((Node *)this, rOperand);
	lOperand->print(printer);
	rOperand->print(printer);
}

Expression *newAffectationIncrement(Variable *lvalue, IncrementType type)
{
	// TODO actually obey the increment type, this is just a placeholder
	BinaryExpression *inc = new BinaryExpression(lvalue, PLUS, new ConstInteger(1));
	return new Affectation(lvalue, inc);
}

FunctionAppel::~FunctionAppel()
{
}

std::string FunctionAppel::printSelf() const
{
	return "Function call: " + *funcName;
}

void FunctionAppel::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);
	if (args)
	{
		printer->addConnection((Node *)this, args);
		args->print(printer);
	}
	else
	{
		printer->addNullConnection((Node *)this, "No args");
	}
}

UnaryExpression::~UnaryExpression()
{
	delete expression;
}

std::string UnaryExpression::printSelf() const
{
	return unaryOpToString(op);
}

void UnaryExpression::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);
	printer->addConnection((Node *)this, expression);
	expression->print(printer);
}

BinaryExpression::~BinaryExpression()
{
	delete lExpression;
	delete rExpression;
}

std::string BinaryExpression::printSelf() const
{
	return binaryOpToString(op);
}

void BinaryExpression::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);

	printer->addConnection((Node *)this, lExpression);
	printer->addConnection((Node *)this, rExpression);

	lExpression->print(printer);
	rExpression->print(printer);
}

std::string binaryOpToString(BinaryOperator op)
{
	switch (op)
	{
		case PLUS:
			return "+";
		case MINUS:
			return "-";
		case MULT:
			return "*";
		case DIV:
			return "/";
		case MODULO:
			return "%";
		case LT:
			return "<";
		case LE:
			return "<=";
		case GT:
			return ">";
		case GE:
			return ">=";
		case EQ:
			return "==";
		case NE:
			return "!=";
		case AND:
			return "&&";
		case OR:
			return "||";
		case COMMA:
			return ",";
		default:
			return "";
	}
}

std::string unaryOpToString(UnaryOperator op)
{
	switch (op)
	{
		case NEG:
			return "-";
		case POS:
			return "+";
		case EXCLAMATION:
			return "!";
		default:
			return "";
	}
}
