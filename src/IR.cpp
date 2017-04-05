#include "IR.h"
#include <string>
void CFG::gen_asm(std::ostream & o)
{

}
std::string CFG::IR_reg_to_asm(std::string reg)
{
	int offset=SymbolIndex[reg];
	return std::to_string(offset)+"(%rbp)";
}
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

std::string CFG::new_BB_name()
{
	std::string name = "BB_" + std::to_string(nextBBnumber);
	nextBBnumber++;
	return name;
}

std::string CFG::getNewVariableName()
{
	int nb = nextFreeSymbolIndex;
	nextFreeSymbolIndex++;
	return "t_" + std::to_string(nb);
}

void BasicBlock::gen_asm(std::ostream & o)
{
	for (IRInstr* i : instrs)
	{
		i->gen_asm(o);
	}
	if (exit_true==nullptr)
	{
		//genEpilogue(o);
	}
	IRInstr::Operation op = instrs[instrs.size() - 1]->getOp();
	/*if (exit_false!=nullptr && op !=IRInstr::Operation::cmp_eq
		&& op != IRInstr::Operation::cmp_ne	&& op != IRInstr::Operation::cmp_lt
		&& op != IRInstr::Operation::cmp_le && op != IRInstr::Operation::cmp_gt
		&& op != IRInstr::Operation::cmp_ge)
	
	{

	}*/
}

void BasicBlock::add_IRInstr(IRInstr::Operation op, Type t, std::vector<std::string> params)
{
	instrs.push_back(new IRInstr(this, op, t, params));
}

void IRInstr::gen_asm(std::ostream & o)
{
}
