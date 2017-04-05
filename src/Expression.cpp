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

Type Variable::solveScopes(std::deque<SymbolTable*>* environments, int *varCounter, CFG *cfg, ErrorList &errors)
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
		std::stringstream err;
		err << "Error: variable not defined " << id;
		errors.addError(err.str());
	}
	return NOTYPE;
}

std::string Variable::buildIR(CFG * cfg)
{
	return name;//car on ne gère pas les tableaux
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

Type ConstInteger::solveScopes(std::deque<SymbolTable*>* environments, int *varCounter, CFG *cfg, ErrorList &errors)
{
	// nothing to do
	return INT64;
}

std::string ConstInteger::buildIR(CFG * cfg)
{
	std::string name = cfg->create_new_tempvar(INT64);
	std::vector<std::string> operands = { name,std::to_string(value) };
	cfg->addInstruction(IRInstr::ldconst, INT64, operands);

	return std::string();
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

Type ConstCharacter::solveScopes(std::deque<SymbolTable*>* environments, int *varCounter, CFG *cfg, ErrorList &errors)
{
	// nothing to do
	return CHAR;
}

std::string ConstCharacter::buildIR(CFG * cfg)
{
	return std::string();
}

void ConstCharacter::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);
}

std::string AffectationCompound::printSelf() const
{
	return binaryOpToString(op) + "=";
}

Type AffectationCompound::solveScopes(std::deque<SymbolTable*>* environments, int *varCounter, CFG *cfg, ErrorList &errors)
{
	Type ltype = this->lvalue->solveScopes(environments, varCounter, cfg, errors);
	Type rtype = this->rvalue->solveScopes(environments, varCounter,cfg, errors);

	if (rtype == ltype)
		return ltype;
	else
		errors.addError("Error: operand types do not match in affectation");
	return NOTYPE;
}

std::string AffectationCompound::buildIR(CFG * cfg)
{
	return std::string();
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
		default:
			return "";
	}
}

Type AffectationIncrement::solveScopes(std::deque<SymbolTable*>* environments, int *varCounter, CFG *cfg, ErrorList &errors)
{
	return this->lvalue->solveScopes(environments, varCounter,cfg, errors);
}

std::string AffectationIncrement::buildIR(CFG * cfg)
{
	return std::string();
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

Type Affectation::solveScopes(std::deque<SymbolTable*>* environments, int *varCounter, CFG *cfg, ErrorList &errors)
{
	Type ltype = this->lOperand->solveScopes(environments, varCounter,cfg, errors);
	Type rtype = this->rOperand->solveScopes(environments, varCounter,cfg, errors);

	if (rtype == ltype)
		return ltype;
	else
		errors.addError("Error: operand types do not match in affectation ");
	return NOTYPE;
}

std::string Affectation::buildIR(CFG * cfg)
{
	return std::string();
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

Type FunctionAppel::solveScopes(std::deque<SymbolTable*>* environments, int *varCounter, CFG *cfg, ErrorList &errors)
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
			Type t = ((Expression*)((*argsCall)[i]))->solveScopes(environments, varCounter,cfg, errors);
			VarDecl* b = (VarDecl*)((*argsDef)[i]);
			identical = (t == b->getVarType());
			i++;
		}
		if (!identical)
		{
			std::stringstream err;
			err << "Error: definition of function args do not match with function call: " <<id;
			errors.addError(err.str());
			return NOTYPE;
			//break or let the execution continue?
		}

		if (ref->getType() == FUNC_DEF) {
			// FUNC FOUND -> BUILD ASM
			return ref->solveScopes(environments, varCounter,nullptr, errors);
		}
		else
		{
			//ERROR : FUNC NOT DEFINED
			//TODO : consider forward declarations ?
		}
	}
	else {
		std::stringstream err;
		err << "Error: undeclared function " << id;
		errors.addError(err.str());
	}

	return NOTYPE;
}

std::string FunctionAppel::buildIR(CFG * cfg)
{
	return std::string();
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

Type UnaryExpression::solveScopes(std::deque<SymbolTable*>* environments, int *varCounter, CFG *cfg, ErrorList &errors)
{
	return this->expression->solveScopes(environments, varCounter,cfg, errors);
}

std::string UnaryExpression::buildIR(CFG * cfg)
{
	return std::string();
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

Type BinaryExpression::solveScopes(std::deque<SymbolTable*>* environments, int *varCounter, CFG *cfg, ErrorList &errors)
{
	Type ltype = this->lExpression->solveScopes(environments, varCounter,cfg, errors);
	Type rtype = this->rExpression->solveScopes(environments, varCounter,cfg, errors);

	if (rtype == ltype)
	{
		return ltype;
	}
	else
	{
		std::stringstream err;
		err << "Error: operand types do not match in binary expression of operator: " << op;
		errors.addError(err.str());
		return NOTYPE;
	}
}

std::string BinaryExpression::buildIR(CFG * cfg)
{
	std::string reg1 = lExpression->buildIR(cfg);
	std::string reg2 = rExpression->buildIR(cfg);
	std::string reg3 = cfg->create_new_tempvar(INT64);//typage des noeuds intermédiaires?
	IRInstr::Operation o;
	switch (op)
	{
	case PLUS:
		o = IRInstr::add;
	break;
	case MINUS:
		o = IRInstr::sub;
		break;
	case MULT:
		o = IRInstr::mult;
		break;
	case DIV:
		o = IRInstr::div;
		break;
	case MODULO:
		o = IRInstr::mod;
		break;
	case LT:
		o = IRInstr::cmp_lt;
		break;
	case LE:
		o = IRInstr::cmp_le;
		break;
	case GT:
		o = IRInstr::cmp_gt;
		break;
	case GE:
		o = IRInstr::cmp_ge;
		break;
	case EQ:
		o = IRInstr::cmp_eq;
		break;
	case NE:
		o = IRInstr::cmp_ne;
		break;


	}

	cfg->addInstruction(o, INT64, { reg3,reg1,reg2 });
	return reg3;
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
