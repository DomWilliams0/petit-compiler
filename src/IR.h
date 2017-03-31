#pragma once
#ifndef IR.H
#define IR.H

#include <vector>
#include <string>
#include <iostream>
#include <map>

#include "Element.h"

class IRInstr {
public:
	typedef enum {
//unary operation
		pre_increment,
		pre_decrement,
		post_increment,
		post_decrement,
		parenthesis, // todo : to delete?
		unary_plus,
		unary_minus,
		unary_not,
//binary operation
		add,
		sub,
		mul,
		div,
		modulus,
		equal,
		greater,
		greaterOrEqual,
		lower,
		lowerOrEqual,
		notEqual,
		assign,
		add_Assign,
		sub_Assign,
		mul_Assign,
		div_Assign,
		mod_Assign,
		read, // read on memory
		write, // write on memory
		call_function
	} Operation;

	//todo : to delete?
	typedef enum {
		unary_expr,
		binary_expr,
		assign,
		assignIncrement
	} Type;
	
	IRInstr(Operation op, Type t, std::vector<std::string> ls_param);
	void gen_codeAsm(std::ostream &o);

protected:
	BasicBlock * bb;
	Operation op;
	Type type;
	std::vector<std::string> params;
};

class BasicBlock {
public:
	BasicBlock();
	void gen_codeAsm(std::ostream o);
	//                                      ^^^^^^^^^^^^^ todo: to modify?
	void add_IRInstr(IRInstr::Operation op, IRInstr::Type type, std::vector<std::string> params);
protected:
	CFG* cfg;
	std::vector<std::string> ls_IRInstr;
	BasicBlock* exit_true;  /**< pointer to the next basic block, true branch. If nullptr, return from procedure */
	BasicBlock* exit_false; /**< pointer to the next basic block, false branch. If null_ptr, the basic block ends with an unconditional jump */
};

// Le "Control Flow Graph" qui represente l'execution sequentielle des "basic blocks" qui sont chacun une sequence d'instructions.
class CFG {
public:
	//CFG(AST * ast);
	//AST * ast;
	void add_bb(BasicBlock* bb);

	void gen_codeAsm(std::ostream & o);
	std::string IR_reg_to_asm(std::string reg); /**< helper method: inputs a IR reg or input variable, returns e.g. "-24(%rbp)" for the proper value of 24 */
	void gen_asm_prologue(std::ostream& o);
	void gen_asm_epilogue(std::ostream& o);

	// symbol table methods
	void add_to_symbol_table(std::string name, Type t);
	std::string create_new_tempvar(Type t);
	int get_var_index(std::string name);
	Type get_var_type(std::string name);

	// basic block management
	std::string new_BB_name();
	BasicBlock* current_bb;

protected:
	std::map <std::string, Type> SymbolType; /**< part of the symbol table  */
	std::map <std::string, int> SymbolIndex; /**< part of the symbol table  */
	int nextFreeSymbolIndex; /**< to allocate new symbols in the symbol table */
	int nextBBnumber; /**< just for naming */

	std::vector <BasicBlock*> ls_bbs; /**< all the basic blocks of this CFG*/
};
#endif // !IR.H
