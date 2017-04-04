#include "IR.h"
#include <string>
void CFG::add_to_symbol_table(std::string name, Type t)
{
	SymbolType[name] = t;
	SymbolIndex[name] = -8 * nextFreeSymbolIndex;
	nextFreeSymbolIndex++;
}

std::string CFG::create_new_tempvar(Type t)
{
	std::string name= "t_" + std::to_string(nextFreeSymbolIndex);
	add_to_symbol_table(name, t);
	return name;
}

int CFG::get_var_index(std::string name)
{
	return SymbolIndex[name];
}

Type CFG::get_var_type(std::string name)
{
	return SymbolType[name];
}

void CFG::addInstruction(IRInstr::Operation op, Type t, std::vector<std::string> params)
{
	current_bb->add_IRInstr(op, t, params);
}

std::string CFG::getNewVariableName()
{
	int nb = nextFreeSymbolIndex;
	nextFreeSymbolIndex++;
	return "t_" + std::to_string(nb);
}

void BasicBlock::add_IRInstr(IRInstr::Operation op, Type t, std::vector<std::string> params)
{
	instrs.push_back(new IRInstr(this, op, t, params));
}

