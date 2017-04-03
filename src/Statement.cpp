#include <iostream>
#include <algorithm>
#include <sstream>
#include "Printer.h"
#include "Statement.h"
#include "Interpreter.h"

std::string Cond::printSelf() const
{
	return "If";
}

void Cond::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);

	printer->addConnection((Node *)this, condition);
	printer->addConnection((Node *)this, ifBlock);

	condition->print(printer);
	ifBlock->print(printer);

	if(elseBlock != nullptr)
	{
		printer->addConnection((Node *)this, elseBlock);
		elseBlock->print(printer);
	}

}

void Cond::updateElse(Statement *newElse)
{
	if (elseBlock == nullptr)
		elseBlock = newElse;
}

Type Cond::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{

	this->condition->solveScopes(environments, varCounter);
	SymbolTable* temp = new SymbolTable();
	environments->push_back(temp);
	this->ifBlock->solveScopes(environments, varCounter);
	if(this->elseBlock != nullptr)
		this->elseBlock->solveScopes(environments, varCounter);

	return NOTYPE;
}

std::string For::printSelf() const
{
	return "For";
}

void For::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);

	if (init)
	{
		printer->addConnection((Node *)this, init);
		init->print(printer);
	}
	else
	{
		printer->addNullConnection((Node *)this, "No init block");
	}

	if (cond)
	{
		printer->addConnection((Node *)this, cond);
		cond->print(printer);
	}
	else
	{
		printer->addNullConnection((Node *)this, "No condition block");
	}

	if (inc)
	{
		printer->addConnection((Node *)this, inc);
		inc->print(printer);
	}
	else
	{
		printer->addNullConnection((Node *)this, "No post block");
	}

	printer->addConnection((Node *)this, block);
	block->print(printer);
}

std::string Iter::printSelf() const
{
	return "While";
}

void Iter::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);

	printer->addConnection((Node *)this, condition);
	printer->addConnection((Node *)this, iterBlock);

	condition->print(printer);
	iterBlock->print(printer);
}
Type Block::solveScopes(std::deque<SymbolTable*>* environments, int* varCounter)
{
	SymbolTable *blockTable = environments->back();
	for (Node *node : *(this->contents)) {
		if (node->getType() == BLOCK) {
			SymbolTable *env = new SymbolTable();
			environments->push_back(env);
		}
		node->solveScopes(environments, varCounter);
	}
	delete environments->back();
	environments->pop_back();
	return NOTYPE;
}

//SymbolTable * Block::computeSymbolTable()
//{
//	std::map<std::string,VarRef> s={};
//	for(int i=0;i<contents->size();++i)
//	{
//		if((*contents)[i]->getType() ==VAR_DECL)
//		{
//			VarDecl *v=(VarDecl*)(*contents)[i];
//			s[v->getIdentifier()]= {v, 8};
//			//TODO : use var counter
//		}
//		else if((*contents)[i]->getType()==VAR_DEF)
//		{
//			VarDef *v=(VarDef*)(*contents)[i];
//			s[v->getIdentifier()]={v, 8};
//		}
//	}
//	SymbolTable *table = new SymbolTable();
//	table->vars = s;
//
//	return table;
//}

std::string Block::printSelf() const
{
	std::stringstream out;
	out << "Block (" << (contents ? contents->size() : 0) << " size)";
	return out.str();
}

void Block::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);
	if (contents != nullptr)
	{
		for (Node *n : *contents)
		{
			printer->addConnection((Node *)this, n);
			n->print(printer);
		}
	}
}

void Block::createBlocks()
{
	if (contents == nullptr)
		return;

	bool instr = false;
	for(size_t i=0;i<contents->size();++i)
	{
		ElementType type=(ElementType)(*contents)[i]->getType();
		switch(type)
		{
			case VAR_DECL:
			case VAR_DEF:
			{
				if(instr)
				{
					Block* b=new Block(new std::vector<Node*>(contents->begin()+i,contents->end()));

					//delete contents->at(i - 1);
					contents->erase(contents->begin() + i , contents->end());

					b->createBlocks();
					contents->push_back(b);
					return;
				}
				break;
			}
			case COND:
			{
				Cond* con=( Cond* )( (*contents)[i] ) ;
				((Block*)(con->getIfBlock()))->createBlocks();

				if (con->getElseBlock() != nullptr)
					((Block*)(con->getElseBlock()))->createBlocks();
				instr=true;
				break;
			}
			case ITER:
			{
				Iter* iter=(Iter* ) ( (*contents)[i] ) ;
				((Block*)(iter->getIterBlock()))->createBlocks();
				instr=true;
				break;
			}
			case FOR_ITER:
			{
				For *f = (For *)( (*contents)[i] ) ;
				((Block*)(f->getBlock()))->createBlocks();
				instr=true;
				break;
			}

			default:
				instr=true;
				break;
		}
	}
}
std::string Return::printSelf() const
{
	return "Return";
}

void Return::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);

	if (value)
	{
		printer->addConnection((Node *)this, value);
		value->print(printer);
	}
}

Block::~Block()
{
	if (contents != nullptr)
	{
		std::for_each(contents->begin(), contents->end(), [](Node *n) { delete n; });
		delete contents;
		contents = nullptr;
	}
}

Cond::~Cond()
{
	delete ifBlock;
	delete condition;
	if (elseBlock)
		delete elseBlock;

	ifBlock = nullptr;
	condition = nullptr;
	elseBlock = nullptr;
}

For::~For()
{
	if (init)
		delete init;
	if (cond)
		delete cond;
	if (inc)
		delete inc;
	delete block;

	init = nullptr;
	cond = nullptr;
	inc = nullptr;
	block = nullptr;
}

Type For::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	SymbolTable* temp = new SymbolTable();
	environments->push_back(temp);
	this->init->solveScopes(environments, varCounter);
	this->cond->solveScopes(environments, varCounter);
	this->inc->solveScopes(environments, varCounter);
	this->block->solveScopes(environments, varCounter);

	return NOTYPE;
}

Iter::~Iter()
{
	delete condition;
	delete iterBlock;

	condition = nullptr;
	iterBlock = nullptr;
}

Type Iter::solveScopes(std::deque<SymbolTable*>* environments, int * varCounter)
{
	this->condition->solveScopes(environments, varCounter);
	SymbolTable* temp = new SymbolTable();
	environments->push_back(temp);	
	this->iterBlock->solveScopes(environments, varCounter);
	return NOTYPE;
}

Return::~Return()
{
	if (value)
		delete value;

	value = nullptr;
}

Type Return::solveScopes(std::deque<SymbolTable*>*, int * varCounter)
{
	return NOTYPE;
}

