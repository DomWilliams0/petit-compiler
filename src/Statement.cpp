#include <iostream>
#include "Statement.h"

void Cond::print() const
{
	std::cout << "if(";
	condition->print();
	std::cout << ")" << std::endl;
	ifBlock->print();
	if(elseBlock != nullptr)
	{
		std::cout << "else";
		elseBlock->print();
	}

}

void Cond::updateElse(Statement *newElse)
{
	if (elseBlock == nullptr)
		elseBlock = newElse;
}

void Iter::print() const
{
	std::cout << "while(";
	condition->print();
	std::cout << ")" << std::endl;
	iterBlock->print();
}

void Block::print() const
{
	std::cout << "{";
	if (contents != nullptr)
	{
		std::cout << std::endl;
		for(size_t i = 0; i < contents->size(); ++i)
		{
			if (i != 0)
				std::cout << std::endl;
			contents->at(i)->print();
		}
	}
	std::cout << "}" << std::endl;
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
void Return::print() const
{
	std::cout << "return";
	if (value)
	{
		std::cout << " ";
		value->print();
	}

	std::cout << ";";
}
