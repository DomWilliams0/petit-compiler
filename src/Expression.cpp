#include <algorithm>
#include <sstream>
#include "Printer.h"
#include "Expression.h"
#include "Interpreter.h"

std::string Variable::printSelf() const
{
	std::stringstream out;
	out << "Var " << name;
	return out.str();
}

Type Variable::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	std::string id = this->name;
	int var_id = -777;
	Node *ref;

	for (int i = environments->size() - 1; i >= 0; i--)
	{

		if ((((*environments)[i])->vars).find(id) != (((*environments)[i])->vars).end()) {
			var_id = (((*environments)[i])->vars)[id].id;
			ref = (((*environments)[i])->vars)[id].ref;
			break;
		}
	}

	if (var_id != -777) {
		// VAR FOUND -> BUILD ASM
		if (ref->getType() == VAR_DECL) {
			return ((VarDecl*)(ref))->getVarType();
		}
		else if (ref->getType() == VAR_DEF) {
			return ((VarDef*)(ref))->getVarType();
		}
	}
	else {
		std::cerr << "Error: variable not defined " << id << std::endl;
	}
	return NOTYPE;
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

Type ConstInteger::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	// nothing to do
	return INT64;
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

Type ConstCharacter::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	// nothing to do
	return CHAR;
}

void ConstCharacter::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);
}

std::string AffectationCompound::printSelf() const
{
	return binaryOpToString(op) + "=";
}

Type AffectationCompound::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	Type ltype = this->lvalue->solveScopes(environments, varCounter);
	Type rtype = this->rvalue->solveScopes(environments, varCounter);

	if (rtype == ltype)
		return ltype;
	else
		std::cerr << "Error: operand types do not match in affectation " << std::endl;
	return NOTYPE;
}

void AffectationCompound::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);

	printer->addConnection((Node *)this, lvalue);
	printer->addConnection((Node *)this, rvalue);
	lvalue->print(printer);
	rvalue->print(printer);
}

std::string AffectationIncrement::printSelf() const
{
	switch (op)
	{
	case POST_INC:
		return "++ post";
	case PRE_INC:
		return "++ pre";
	case POST_DEC:
		return "-- post";
	case PRE_DEC:
		return "-- pre";
	default:
		return "";
	}
}

Type AffectationIncrement::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	return this->lvalue->solveScopes(environments, varCounter);
}

void AffectationIncrement::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);

	printer->addConnection((Node *)this, lvalue);
	lvalue->print(printer);
}

std::string Affectation::printSelf() const
{
	return "=";
}

Type Affectation::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	Type ltype = this->lOperand->solveScopes(environments, varCounter);
	Type rtype = this->rOperand->solveScopes(environments, varCounter);

	if (rtype == ltype)
		return ltype;
	else
		std::cerr << "Error: operand types do not match in affectation " << std::endl;
	return NOTYPE;
}

void Affectation::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);

	printer->addConnection((Node *)this, lOperand);
	printer->addConnection((Node *)this, rOperand);
	lOperand->print(printer);
	rOperand->print(printer);
}

FunctionAppel::FunctionAppel(const std::string &name, Expression *args) : funcName(name)
{
	if (args != nullptr)
	{
		Expression *current = args;
		while (true)
		{
			if (current->getType() == BINARY)
			{
				BinaryExpression *bin = (BinaryExpression *)current;
				if (bin->getOperator() == COMMA)
				{
					this->args.push_back(bin->getRightExpression());
					current = bin->getLeftExpression();
					continue;
				}
			}

			this->args.push_back(current);
			break;
		}

		std::reverse(this->args.begin(), this->args.end());
	}
}

std::string FunctionAppel::printSelf() const
{
	return "Function call: " + funcName;
}

Type FunctionAppel::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	std::string id = this->funcName;
	Node * ref = nullptr;

	for (int i = environments->size() - 1; i >= 0; i--)
	{

		if ((((*environments)[i])->funct).find(id) != (((*environments)[i])->funct).end()) {
			ref = (((*environments)[i])->funct)[id];
			break;
		}
	}

	if (ref != nullptr) {
		std::vector<Element*>* argsDef;
		std::vector<Expression*>* argsCall = &(this->args);

		if (ref->getType() == FUNC_DEF)
		{
			argsDef = ((FuncDef*)ref)->getDecl()->getArgs();
		}
		else
		{
			argsDef = ((FuncDecl*)ref)->getArgs();
		}


		int size = argsCall->size();
		bool identical = (argsDef->size() == size);
		int i = 0;

		while (identical && i < size)
		{
			Type t = ((Expression*)((*argsCall)[i]))->solveScopes(environments, varCounter);
			VarDecl* b = (VarDecl*)((*argsDef)[i]);
			identical = (t == b->getVarType());
			i++;
		}
		if (!identical)
		{
			std::cerr << "Error: definition of function args do not match with function call: " << id << std::endl;
			return NOTYPE;
			//break or let the execution continue?
		}

		if (ref->getType() == FUNC_DEF) {
			// FUNC FOUND -> BUILD ASM
			return ref->solveScopes(environments, varCounter);
		}
		else
		{
			//ERROR : FUNC NOT DEFINED
			//TODO : consider forward declarations ?
		}
	}
	else {
		std::cerr << "Error: undeclared function " << id << std::endl;
	}

	return NOTYPE;
}


void FunctionAppel::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);
	if (args.empty())
	{
		printer->addNullConnection((Node *)this, "No args");
	}
	else
	{
		for (Expression *arg : args)
		{
			printer->addConnection((Node *)this, arg);
			arg->print(printer);
		}
	}
}

std::string UnaryExpression::printSelf() const
{
	return unaryOpToString(op);
}

Type UnaryExpression::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	return this->expression->solveScopes(environments, varCounter);
}

void UnaryExpression::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);
	printer->addConnection((Node *)this, expression);
	expression->print(printer);
}

std::string BinaryExpression::printSelf() const
{
	return binaryOpToString(op);
}

Type BinaryExpression::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	Type ltype = this->lExpression->solveScopes(environments, varCounter);
	Type rtype = this->rExpression->solveScopes(environments, varCounter);

	if (rtype == ltype)
		return ltype;
	else
		std::cerr << "Error: operand types do not match in binary expression of operator: " << op << std::endl;
	return NOTYPE;
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

Variable::~Variable()
{
	if (index)
		delete index;

	index = nullptr;
}

Affectation::~Affectation()
{
	delete lOperand;
	delete rOperand;

	lOperand = nullptr;
	rOperand = nullptr;
}

AffectationIncrement::~AffectationIncrement()
{
	delete lvalue;
}

AffectationCompound::~AffectationCompound()
{
	delete lvalue;
	delete rvalue;
}

FunctionAppel::~FunctionAppel()
{
	std::for_each(args.begin(), args.end(), [](Expression *e) { delete e; });
	args.clear();
}

UnaryExpression::~UnaryExpression()
{
	delete expression;
	expression = nullptr;
}

BinaryExpression::~BinaryExpression()
{
	delete lExpression;
	delete rExpression;

	lExpression = rExpression = nullptr;
}