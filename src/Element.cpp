#include <sstream>
#include <algorithm>
#include "Printer.h"
#include "Element.h"
#include "Statement.h"
#include "Interpreter.h"

std::string typeToString(Type type)
{
	switch(type)
	{
		case INT32:
			return "int32_t ";
		case INT64:
			return "int64_t ";
		case CHAR:
			return "char ";
		case VOID:
			return "void ";
		default:
			return "";
	}
}
void Document::createBlocks()
{
	for(int i=0;i<elements.size();++i)
	{
		if(elements[i]->getType()==FUNC_DEF)
		{
			FuncDef* fallourd=static_cast < FuncDef* > ( elements[i] ) ;
			fallourd->getBlock()->createBlocks();
		}
	}
}


std::string VarDecl::printSelf() const
{
	std::stringstream out;
	out << "Var Declaration: " << typeToString(type) << identifier << "[" << arraySize << "]";
	return out.str();
}

void VarDecl::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);
}

void VarDecl::updateType(Type type)
{
	if (this->type == PLACEHOLDER_TYPE)
		this->type = type;
}


Type VarDecl::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	if ((environments->back()->vars).find(identifier) == (environments->back()->vars).end())
	{
		(environments->back()->vars)[identifier] = { this,(*varCounter)++ };
	}
	else
	{
		std::cerr << "Error: redeclaration of variable: " << this->identifier << std::endl;
	}
	return NOTYPE;
}

std::string VarDeclList::printSelf() const
{
	std::stringstream out;
	out << declarations->size() << " var decl(s)";
	return out.str();
}

void VarDeclList::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);
	for (Element *e : *declarations)
	{
		printer->addConnection((Node *)this, e);
		e->print(printer);
	}
}

void VarDeclList::addDeclaration(VarDecl *decl)
{
	decl->updateType(type);
	declarations->push_back(decl);
}

Type VarDeclList::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	for (Element* e : *declarations)
	{
		e->solveScopes(environments, varCounter);
	}
	return NOTYPE;
}

std::string VarDef::printSelf() const
{
	return "Var Definition";
}

void VarDef::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);

	printer->addConnection((Node *)this, decl);
	printer->addConnection((Node *)this, value);

	decl->print(printer);
	value->print(printer);
}

void VarDef::updateType(Type type)
{
	decl->updateType(type);
}

Type VarDef::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	if ((environments->back()->vars).find(identifier) == (environments->back()->vars).end())
	{
		if (this->getVarType() != this->getValue()->solveScopes(environments, varCounter))
		{
			std::cerr << "Error: variable " << identifier << " initialized with mismatching type expression" << std::endl;
		}

		(environments->back()->vars)[identifier] = { this,(*varCounter)++ };
	}
	else
	{
		std::cerr << "Error: redefinition of variable: " << this->identifier << std::endl;
	}
	
	return NOTYPE;
}

std::string FuncDecl::printSelf() const
{
	return "Function Declaration: " + typeToString(functionType) + identifier;
}

Type FuncDecl::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	if ((environments->back()->funct).find(identifier) == (environments->back()->funct).end())
	{
		(environments->back()->funct)[identifier] = { this };
	}
	else
	{
		
	}
	
	return functionType;
}

void FuncDecl::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);

	if (args->empty())
	{
		printer->addNullConnection((Node *)this, "No args");
	}
	else
	{
		for (Element *arg : *args)
		{
			printer->addConnection((Node *)this, arg);
			arg->print(printer);
		}
	}
}

std::string FuncDef::printSelf() const
{
	return "Function Definition: " + identifier;
}

void FuncDef::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);

	printer->addConnection((Node *)this, (Node *)&decl);
	decl.print(printer);

	printer->addConnection((Node *)this, block);
	block->print(printer);
}

Type FuncDef::solveScopes(std::deque<SymbolTable*>* environments,int * varCounter){
	SymbolTable *blockTable = new SymbolTable();
	auto it= (environments->back()->funct).find(identifier);
	if (it == (environments->back()->funct).end())
	{
		(environments->back()->funct)[identifier] = { this };
		for (Element * n : *(this->decl.getArgs())) {
			VarDecl* temp = (VarDecl*)n;
			(blockTable->vars)[temp->getIdentifier()] = { temp,(*varCounter)++ };
		}
		
		environments->push_back(blockTable);
		block->solveScopes(environments, varCounter);
	}
	else
	{
		if (it->second->getType() == FUNC_DECL)
		{
			std::vector<Element*>* argsDecl = (((FuncDecl*)(it->second))->getArgs());
			std::vector<Element*>* argsDef = decl.getArgs();
			int size = argsDecl->size();
			bool identical = (argsDef->size() == size);
			int i = 0;
			
			while (identical && i < size)
			{
				VarDecl* a = (VarDecl*)((*argsDecl)[i]);
				VarDecl* b = (VarDecl*)((*argsDef)[i]);
				identical = ( a->getVarType() == b->getVarType() );
				i++;
			}
			if (!identical)
			{
				std::cerr << "Error: definition of function args do not match with previous declaration: " << this->identifier << std::endl;
			}
		}
		std::cerr << "Error: redefinition of function: " << this->identifier << std::endl;
	}
	return decl.getFuncType();
}

std::string Document::printSelf() const
{
	return "Document";
}

void Document::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);
	for (Element *e : elements)
	{
		printer->addConnection((Node *)this, e);
		e->print(printer);
	}
}

void Document::addElement(Element *e)
{
	if (e != nullptr)
		elements.push_back(e);
}

Document::~Document()
{
	for (Element *e : elements)
		delete e;
	elements.clear();
}

VarDecl::~VarDecl()
{
}

VarDeclList::~VarDeclList()
{
	std::for_each(declarations->begin(), declarations->end(), [](Element *e) { delete e; });
	delete declarations;
	declarations = nullptr;
}

VarDef::~VarDef()
{
	delete decl;
	delete value;
	value = nullptr;
	decl = nullptr;
}

FuncDef::~FuncDef()
{
	delete block;
	block = nullptr;
}

FuncDecl::~FuncDecl()
{
	std::for_each(args->begin(), args->end(), [](Element *e) { delete e; });
	delete args;
	args = nullptr;
}

