#include <iostream>
#include "Printer.h"
#include "Statement.h"

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

std::string For::printSelf() const
{
	return "For";
}

void For::print(GraphPrinter *printer) const
{
	printer->makeNode((Node *)this);

	printer->addConnection((Node *)this, init);
	printer->addConnection((Node *)this, cond);
	printer->addConnection((Node *)this, inc);
	printer->addConnection((Node *)this, block);

	init->print(printer);
	cond->print(printer);
	inc->print(printer);
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

std::string Block::printSelf() const
{
	return "Block"; // TODO add count
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
	std::vector<Node*>tempEls = std::vector<Node*>();
	std::vector<Node*>newEls = std::vector<Node*>();
	bool instr = false;
	for(int i=0;i<contents->size();++i)
	{
		int type=(*contents)[i]->getType();
		switch(type)
		{
			case VAR_DEF:
			{
				if(instr)
				{
					Block* b=new Block(new std::vector<Node*>(contents->begin()+i-1,contents->end()));
					contents=new std::vector<Node*>(contents->begin(),contents->begin()+i-1);
					b->createBlocks();
					contents->push_back(b);
					return;
				}
				break;
			}
			case VAR_DECL:
			{
				if(instr)
				{
					Block* b=new Block(new std::vector<Node*>(contents->begin()+i-1,contents->end()));
					contents=new std::vector<Node*>(contents->begin(),contents->begin()+i-1);
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
			default:
			{
				instr=true;
				break;
			}
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
