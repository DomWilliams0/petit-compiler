#ifndef IR_H
#define IR_H

#include <vector>
#include <map>
#include <ostream>
#include <initializer_list>
//#include "Element.h"

// Declarations from the parser -- replace with your own


class BasicBlock;
class CFG;
class FuncDef;
enum Type { INT32, INT64, CHAR, VOID, PLACEHOLDER_TYPE, NOTYPE };
/*enum Type {
	unary_expr,
	binary_expr,
	assignType,
	assignIncrement
};*/
//! The class for one 3-address instruction
class IRInstr {

public:
	/** The instructions themselves -- feel free to subclass instead */
	typedef enum {
		ldconst,
		add,
		sub,
		mul,
		rmem,
		wmem,
		call,
		cmp_eq,
		cmp_lt,
		cmp_le
	} Operation;


	/**  constructor */
	IRInstr(BasicBlock* bb_, Operation op_, Type t_, std::vector<std::string> params_) :bb(bb_), op(op_), t(t_), params(params) {};

	/** Actual code generation */
	void gen_asm(std::ostream &o); /**< x86 assembly code generation for this IR instruction */

private:
	BasicBlock* bb; /**< The BB this instruction belongs to, which provides a pointer to the CFG this instruction belong to */
	Operation op;
	Type t;
	std::vector<std::string> params; /**< For 3-op instrs: d, x, y; for ldconst: d, c;  For call: label, d, params;  for wmem and rmem: choose yourself */
						   // if you subclass IRInstr, each IRInstr subclass has its parameters and the previous (very important) comment becomes useless: it would be a better design. 
};






/**  The class for a basic block */

/* A few important comments.
IRInstr has no jump instructions:
assembly jumps are generated as follows in BasicBlock::gen_asm():
1/ a cmp_* comparison instructions, if it is the last instruction of its block,
generates an actual assembly comparison followed by a conditional jump to the exit_false branch
If it is not the last instruction of its block, it behaves as an arithmetic two-operand instruction (add or mult)
2/ BasicBlock::gen_asm() first calls IRInstr::gen_asm() on all its instructions, and then
if  exit_true  is a  nullptr, it generates the epilogue
if  exit_false is not a nullptr, and the last instruction is not a cmp, it generates two conditional branches based on the value of the last variable assigned
otherwise it generates an unconditional jmp to the exit_true branch
*/

class BasicBlock {
public:
	BasicBlock(CFG* cfg, std::string entry_label);
	void gen_asm(std::ostream &o); /**< x86 assembly code generation for this basic block (very simple) */

	void add_IRInstr(IRInstr::Operation op, Type t, std::vector<std::string> params);

	// No encapsulation whatsoever here. Feel free to do better.
	BasicBlock* exit_true;  /**< pointer to the next basic block, true branch. If nullptr, return from procedure */
	BasicBlock* exit_false; /**< pointer to the next basic block, false branch. If null_ptr, the basic block ends with an unconditional jump */
	std::string label; /**< label of the BB, also will be the label in the generated code */
	CFG* cfg; /** < the CFG where this block belongs */
	std::vector<IRInstr*> instrs; /** < the instructions themselves. */
protected:


};




/** The class for the control flow graph, also includes the symbol table */

/* A few important comments:
The entry block is the one with the same label as the AST function name.
(it could be the first of bbs, or it could be defined by an attribute value)
The exit block is the one with both exit pointers equal to nullptr.
(again it could be identified in a more explicit way)

*/
class CFG {
public:
	CFG(FuncDef* ast) :nextFreeSymbolIndex(1) {};

	FuncDef* ast; /**< The AST this CFG comes from */

	void add_bb(BasicBlock* bb);

	// x86 code generation: could be encapsulated in a processor class in a retargetable compiler
	void gen_asm(std::ostream& o);
	std::string IR_reg_to_asm(std::string reg); /**< helper method: inputs a IR reg or input variable, returns e.g. "-24(%rbp)" for the proper value of 24 */
	void gen_asm_prologue(std::ostream& o);
	void gen_asm_epilogue(std::ostream& o);

	// symbol table methods
	void add_to_symbol_table(std::string name, Type t);
	std::string create_new_tempvar(Type t);
	int get_var_index(std::string name);
	Type get_var_type(std::string name);
	void addInstruction(IRInstr::Operation op, Type t, std::vector<std::string> params);
	// basic block management
	std::string new_BB_name();
	BasicBlock* current_bb;
	std::string getNewVariableName();
protected:
	std::map <std::string, Type> SymbolType; /**< part of the symbol table  */
	std::map <std::string, int> SymbolIndex; /**< part of the symbol table  */
	int nextFreeSymbolIndex; /**< to allocate new symbols in the symbol table */
	int nextBBnumber; /**< just for naming */

	std::vector <BasicBlock*> bbs; /**< all the basic blocks of this CFG*/
};


#endif