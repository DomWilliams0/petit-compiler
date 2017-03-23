#include <algorithm>
#include "Element.h"
#include "Statement.h"

void printType(Type type)
{
	switch(type)
	{
		case INT32:
			std::cout << "int32_t ";
			break;
		case INT64:
			std::cout << "int64_t ";
			break;
		case CHAR:
			std::cout << "char ";
			break;
		case VOID:
			std::cout << "void ";
			break;
		default:
			break;
	}
}

void VarDecl::print() const
{
	printType(this->type);
	std::cout << identifier;
}

void VarDecl::updateType(Type type)
{
	if (this->type == PLACEHOLDER_TYPE)
		this->type = type;
}

void VarDeclList::print() const
{
	printType(type);

	// TODO fix this and wrap it up in a macro
	for(size_t i = 0; i < declarations->size(); ++i)
	{
		declarations->at(i)->print();
		std::cout << ",";
	}
}

void VarDeclList::addDeclaration(VarDecl *decl)
{
	decl->updateType(type);
	declarations->push_back(decl);
}

void VarDef::print() const
{
	decl->print();
	std::cout << identifier << " = ";
	value->print();
}

void VarDef::updateType(Type type)
{
	decl->updateType(type);
}

void FuncDecl::print() const
{
	printType(functionType);
	std::cout << identifier << "(";

	for(size_t i = 0; i < args->size(); ++i)
	{
		args->at(i)->print();
		std::cout << ",";
	}

	std::cout << ")";
}
void FuncDef::print() const
{
	decl.print();
	block->print();
}

void Document::print() const
{
	std::for_each(elements.begin(), elements.end(), [] (Element *e) { e->print(); });
}

void Document::addElement(Element *e)
{
	if (e != nullptr)
		elements.push_back(e);
}
