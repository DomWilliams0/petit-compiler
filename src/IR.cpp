#include "IR.h"
#include "Element.h"
#include <string>
CFG::CFG(FuncDef * ast)
{
	nextFreeSymbolIndex = 1;
	nextBBnumber = 0;
	bbs.push_back(new BasicBlock(this, this->new_BB_name()));
	current_bb = bbs.back();
	this->label = ast->getIdentifier();
}
void CFG::gen_asm(std::ostream & o)
{
	this->gen_asm_prologue(o);
	for (auto &block : this->bbs)
	{
		block->gen_asm(o);
	}
}
std::string CFG::IR_reg_to_asm(std::string reg)
{
	int offset=SymbolIndex[reg];
	return std::to_string(offset)+"(%rbp)";
}
void CFG::gen_asm_prologue(std::ostream & o)
{
	int offset = this->SymbolIndex.size() * 8;
	o << this->label << ":" << std::endl;
	o << "push %rbp" << std::endl << "movq %rsp, %rbp" << std::endl << "addq $" << std::to_string(offset) << ", %rsp" << std::endl;
}
void CFG::gen_asm_epilogue(std::ostream & o)
{
	o << "leave" << std::endl << "ret" << std::endl;
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
	for (IRInstr* i : *instrs)
	{
		i->gen_asm(o);
	}
	if (exit_true == nullptr)
	{
		this->cfg->gen_asm_epilogue(o);
	}
	/*IRInstr::Operation op = (*instrs)[instrs->size() - 1]->getOp();
	if (exit_false!=nullptr && op !=IRInstr::Operation::cmp_eq
		&& op != IRInstr::Operation::cmp_ne	&& op != IRInstr::Operation::cmp_lt
		&& op != IRInstr::Operation::cmp_le && op != IRInstr::Operation::cmp_gt
		&& op != IRInstr::Operation::cmp_ge)
	
	{

	}*/
}

void BasicBlock::add_IRInstr(IRInstr::Operation op, Type t, std::vector<std::string> params)
{
	this->instrs->push_back(new IRInstr(this, op, t, params));
}

void IRInstr::gen_asm(std::ostream & o)
{
	switch (this->op)
	{
		case ldconst: {
			int offset = bb->cfg->get_var_index(params[0]);
			o << "movq $" << params[1] << " , " << offset << "(%rbp)" << std::endl;
		}
					  break;
		case wmem: {
			int offset = bb->cfg->get_var_index(params[0]);
			int offsetR = bb->cfg->get_var_index(params[1]);
			o << "movq    " << offsetR << "(%rbp), %rax" << std::endl;
			o << "movq    %rax, " << offset << "(%rbp)" << std::endl;
		}
					 break;
		case add: {
			int offset = bb->cfg->get_var_index(params[1]);
			int offsetR = bb->cfg->get_var_index(params[2]);
			int offsetD = bb->cfg->get_var_index(params[0]);
			//std::cerr << "dest is : " << params[0] << std::endl;
			o << "movq    " << offset << "(%rbp), %rdx" << std::endl;
			o << "movq    " << offsetR << "(%rbp), %rax" << std::endl;
			o << "addq    %rdx, %rax" << std::endl;
			o << "movq    %rax, " << offsetD << "(%rbp)" << std::endl;
		}
				  break;
		case sub: {
			int offset = bb->cfg->get_var_index(params[1]);
			int offsetR = bb->cfg->get_var_index(params[2]);
			int offsetD = bb->cfg->get_var_index(params[0]);
			o << "movq    " << offsetR << "(%rbp), %rdx" << std::endl;
			o << "movq    " << offset << "(%rbp), %rax" << std::endl;
			o << "subq    %rdx, %rax" << std::endl;
			o << "movq    %rax, " << offsetD << "(%rbp)" << std::endl;
		}
				  break;
		case mult: {
			int offset = bb->cfg->get_var_index(params[1]);
			int offsetR = bb->cfg->get_var_index(params[2]);
			int offsetD = bb->cfg->get_var_index(params[0]);
			o << "movq    " << offset << "(%rbp), %rdx" << std::endl;
			o << "movq    " << offsetR << "(%rbp), %rax" << std::endl;
			o << "imulq    %rdx, %rax" << std::endl;
			o << "movq    %rax, " << offsetD << "(%rbp)" << std::endl;
		}
				  break;
		case div: {
			int offset = bb->cfg->get_var_index(params[1]);
			int offsetR = bb->cfg->get_var_index(params[2]);
			int offsetD = bb->cfg->get_var_index(params[0]);
			o << "movq    " << offsetR << "(%rbp), %r9" << std::endl;
			o << "movq    " << offset << "(%rbp), %rax" << std::endl;
			o << "cqto" << std::endl;
			o << "idivq    %r9" << std::endl;
			o << "movq    %rax, " << offsetD << "(%rbp)" << std::endl;
		}
				   break;
		case mod: {
			int offset = bb->cfg->get_var_index(params[1]);
			int offsetR = bb->cfg->get_var_index(params[2]);
			int offsetD = bb->cfg->get_var_index(params[0]);
			o << "movq    " << offsetR << "(%rbp), %r9" << std::endl;
			o << "movq    " << offset << "(%rbp), %rax" << std::endl;
			o << "cqto" << std::endl;
			o << "idivq    %r9" << std::endl;
			o << "movq    %rdx, " << offsetD << "(%rbp)" << std::endl;
		}
				  break;
	}
}
