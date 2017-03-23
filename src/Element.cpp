#include <algorithm>
#include "Element.h"
#include "Statement.h"

void printType(Type type)
{
	switch(type)
	{
		case INT32:
			std::cout << "int32_t " << std::endl;
			break;
		case INT64:
			std::cout << "int64_t " << std::endl;
			break;
		case CHAR:
			std::cout << "char " << std::endl;
			break;
		case VOID:
			std::cout << "void " << std::endl;
			break;
		default:
			break;
	}
}

Element::~Element()
{
}

FuncDef:: ~FuncDef()
{
	delete block;
}

VarDecl::~VarDecl()
{

}

FuncDecl::~FuncDecl()
{

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

	std::cout << ");";
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
