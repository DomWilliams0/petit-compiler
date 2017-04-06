#pragma once
#ifndef IR_H
#define IR_H

#include <vector>
#include <string>
#include <iostream>
#include <map>

#include "Element.h"
#include "Expression.h"
#include "Register.h"
#include "Data.h"

class BasicBlock;
class CFG;

class Operand {
public : 
	// we have 2 kinds of operands : a variable or a constant
	enum class OperandType {
		OperandVariable,
		OperandConstant
	};
	Operand(OperandType op_type) : operand_type(op_type) {};
	virtual ~Operand() = 0;
	// type of the concerned operand
protected:
	const OperandType operand_type;
};

class Constant : public Operand {
public:
	static std::unique_ptr<Constant> Create(int64_t value);
	Constant(int64_t value) : Operand(OperandType::OperandConstant), value(value) {};
	~Constant() {};
	// value of the concerned constant
protected:
	int64_t value;
};

class Variable_op : public Operand {
	// Not satisfied with this domination, but class Variable exists already as subclass of Expression...
public:
	//                                                           ^^^^^^^^^^^^ todo: to correct?
	static std::unique_ptr<Variable_op> Create(const std::shared_ptr<const OperandType> variable);
	//                            ^^^^^^^^^^^^^ todo : to correct?
	Variable_op(const std::shared_ptr<const OperandType> variable) : Operand(OperandType::OperandVariable), variable(variable) {};
	~Variable_op() {};
protected:
	// expression of the concerned variable
	const std::shared_ptr<const OperandType> variable;
};

class IRInstr {
public:
		enum class OpType {
		//pre_increment, pre_decrement, post_increment, post_decrement, parenthesis, unary_plus, unary_minus, unary_not.
			unaOp,
		//add, sub, mul, div, modulus, equal, greater, greaterOrEqual, lower, lowerOrEqual, notEqual.
			binOp,
		//call_function.
			callOp,
		//add_Assign, sub_Assign, mul_Assign, div_Assign, mod_Assign,
			assignOp,
		// read on memory
			read, // read on memory
			write, // write on memory
		};

		IRInstr(OpType op_type) : op_type(op_type) {};
		virtual ~IRInstr() = 0;
		// type of the concerned instruction(operation)
		const OpType op_type;

	/*typedef std::shared_ptr<Instruction> sh_Instruction;
	void gen_codeAsm(std::ostream &o);
	virtual std::string toX86() const = 0;

	std::vector<sh_Register> getReadRegisterVector() const;
	std::vector<sh_Register> getWroteRegisterVector() const;
	std::vector<sh_Data> getReadDataVector() const;
	std::vector<sh_Data> getWroteDataVector() const;*/

/*protected:
	BasicBlock * bb;
	OpType op_type;
	Type type;
	std::vector<std::string> params;*/

	/*std::vector<sh_Register> readRegisterVector;
	std::vector<sh_Register> wroteRegisterVector;
	std::vector<sh_Data> readDataVector;
	std::vector<sh_Data> wroteDataVector;*/
};

class CallOp : public IRInstr {
	// Call function "function" with arguments "args" and return value is "ret"
public:
	//                                                                                 ^^^^^^^^^^^^^^ todo : to modify?
	static std::unique_ptr<CallOp> Create(std::shared_ptr<Operand> args, std::shared_ptr<FunctionAppel> function, std::shared_ptr<Variable_op> ret);
	//                                                    ^^^^^^^^^^^^^^ todo : to modify?
	CallOp(std::shared_ptr<Operand> args, std::shared_ptr<FunctionAppel> function, std::shared_ptr<Variable_op> ret) : IRInstr(OpType::callOp), function(function), args(args), ret(ret) {};
	virtual ~CallOp() {};
protected:
	std::shared_ptr<FunctionAppel> function;
	std::shared_ptr<Operand> args;
	std::shared_ptr<Variable_op> ret;
};

class BinOp : public IRInstr {
// Binary Operation with result in "dest", "dest" = "left" "bin_operator" "right"
public:
	enum class BinOperator {
		Add, // left + right
		Sub, // left - right
		Mul, // left * right
		Div, // left / right
		Mod, // left % right
		Equal, // left == right
		Greater, // left > right
		GreaterOrEqual, // left >= right
		Lower, // left < right
		LowerOrEqual, // left <= right
		NotEqual // left != right
	};

	static std::unique_ptr<BinOp> Create(std::shared_ptr<Variable_op> dest, std::shared_ptr<Operand> left, BinOperator bin_operator, std::shared_ptr<Operand> right);
	BinOp(std::shared_ptr<Variable_op> dest, std::shared_ptr<Operand> left, BinOperator bin_operator, std::shared_ptr<Operand> right);
	virtual	~BinOp();
protected:
	std::shared_ptr<Variable_op> dest;
	std::shared_ptr<Operand> left;
	BinOperator bin_operator;
	std::shared_ptr<Variable_op> right;
};

class UnaOp : public IRInstr {
public:
	enum class UnaOperator {
		pre_increment, //++expr 
		pre_decrement, //--expr
		post_increment, //expr++
		post_decrement, //expr--
		unary_plus, //+expr
		unary_minus, //-expr
		unary_not //!expr
	};

	static std::shared_ptr<UnaOp> Create(std::shared_ptr<Variable_op> dest, UnaOperator una_operator, std::shared_ptr<Operand> expr);
	UnaOp(std::shared_ptr<Variable_op> dest, UnaOperator una_operator, std::shared_ptr<Operand> expr);
	virtual ~UnaOp();
protected:
	std::shared_ptr<Variable_op> dest;
	UnaOperator una_operator;
	std::shared_ptr<Operand> expr;
};

class BasicBlock {
public:
	BasicBlock();
	/**< x86 assembly code generation for this basic block (very simple) */
	void gen_codeAsm(std::ostream & o);
	//                                      ^^^^^^^^^^^^^ todo: to modify?
	void add_IRInstr(IRInstr::OpType op_type, std::vector<std::string> params);
protected:
	CFG* cfg;
	std::vector<std::string> ls_IRInstr;
	BasicBlock* exit_true;  /**< pointer to the next basic block, true branch. If nullptr, return from procedure */
	BasicBlock* exit_false; /**< pointer to the next basic block, false branch. If null_ptr, the basic block ends with an unconditional jump */
};

class CFG {
// Le "Control Flow Graph" qui represente l'execution sequentielle des "basic blocks" qui sont chacun une sequence d'instructions.
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
