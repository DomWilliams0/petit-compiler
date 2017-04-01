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

Node* Variable::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	std::string id = this->name;
	int var_id = -777;

	for (int i = environments->size() - 1; i >= 0; i--)
	{
		
		if ((((*environments)[i])->vars).find(id) != (((*environments)[i])->vars).end()) {
			var_id = (((*environments)[i])->vars)[id].id;
			break;
		}
	}

	if (var_id != -777) {
		// VAR FOUND -> BUILD ASM
	}
	else {
		// ERROR : UNDEFINED VAR
	}
	return nullptr;
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

Node* ConstInteger::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	// nothing to do
	return nullptr;
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

Node* ConstCharacter::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	// nothing to do
	return nullptr;
}

void ConstCharacter::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);
}

std::string AffectationCompound::printSelf() const
{
	return binaryOpToString(op) + "=";
}

Node* AffectationCompound::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	this->lvalue->solveScopes(environments, varCounter);
	this->rvalue->solveScopes(environments, varCounter);
	return nullptr;
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
	switch(op)
	{
		case POST_INC:
			return "++ post";
		case PRE_INC:
			return "++ pre";
		case POST_DEC:
			return "-- post";
		case PRE_DEC:
			return "-- pre";
	}
}

Node* AffectationIncrement::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	this->lvalue->solveScopes(environments, varCounter);
	return nullptr;
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

Node* Affectation::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	this->lOperand->solveScopes(environments, varCounter);
	this->rOperand->solveScopes(environments, varCounter);
	return nullptr;
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

Node* FunctionAppel::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	std::string id = this->funcName;
	Node * ref = nullptr;

	for (int i = environments->size() - 1; i >= 0; i--)
	{

		if ((((*environments)[i])->vars).find(id) != (((*environments)[i])->vars).end()) {
			ref = (((*environments)[i])->vars)[id].ref;
			break;
		}
	}

	if (ref != nullptr) {
		if (ref->getType() == FUNC_DEF) {
			// FUNC FOUND -> BUILD ASM
			ref->solveScopes(environments, varCounter);
		}
		else
		{
			//ERROR : FUNC NOT DEFINED
		}
	}
	else {
		// ERROR : UNDECLARED FUNC
	}
	return nullptr;
}

/*std::map<std::string,Element*> FunctionAppel::computeSymbolTable()
{
	std::map<std::string,Element*> s={};
	for(int i=0;i<contents->size();++i)
	{
		if((*contents)[i]->getType() ==VAR_DECL)
		{
			VarDecl *v=(VarDecl*)(*contents)[i];
			s[v->getIdentifier()]=v;
		}
		else if((*contents)[i]->getType()==VAR_DEF)
		{
			VarDef *v=(VarDef*)(*contents)[i];
			s[v->getIdentifier()]=v;
		}
	}
	return s;
}*/

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

Node* UnaryExpression::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	this->expression->solveScopes(environments, varCounter);
	return nullptr;
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

Node* BinaryExpression::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	this->lExpression->solveScopes(environments, varCounter);
	this->rExpression->solveScopes(environments, varCounter);
	return nullptr;
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
	std::for_each(args.begin(), args.end(), [] (Expression *e) { delete e; });
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
