#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <deque>
#include "IR.h"
#include "Interpreter.h"

struct SymbolTable;

class GraphPrinter;

//enum Type { INT32, INT64, CHAR, VOID, PLACEHOLDER_TYPE, NOTYPE };
enum ElementType {VAR_DECLS,VAR_DECL,VAR_DEF,FUNC_DECL,FUNC_DEF,BLOCK,RETURN_STAT,COND,ITER,FOR_ITER,UNKNOWN, DOCUMENT, VAR, CONSTINT, CONSTCHAR, AFFECTATION, AFFECTATION_INC, AFFECTATION_COMPOUND, FUNCAPPEL, UNARY, BINARY};
class Node
{
	public:
		virtual ~Node() {}
		virtual void print(GraphPrinter *) const = 0;
		virtual std::string printSelf() const = 0;
		virtual ElementType getType() const = 0;

		virtual Type solveScopes(std::deque<SymbolTable*>* environments, int *varCounter, CFG *cfg, ErrorList &errors) = 0;
		virtual std::string buildIR(CFG* cfg) = 0;
	protected:
		Node() {}
};

class Block;
class Expression;

struct Value
{
	Type type;
	union
	{
		int32_t i32;
		int64_t i64;
		char c;
	};

	Value(Type type) : type(type), i32(0) {}

	void printValue() const
	{
		switch(type)
		{
			case INT32:
				std::cout << i32 << std::endl;
				break;
			case INT64:
				std::cout << i64 << std::endl;
				break;
			case CHAR:
				std::cout << c << std::endl;
				break;
			default:
				break;
		}
	}
};


std::string typeToString(Type type);

class Element : public Node
{
	public:
		virtual ~Element() {}
		virtual void print(GraphPrinter *) const = 0;
		virtual std::string printSelf() const = 0;
		virtual ElementType getType() const = 0;
		const std::string &getIdentifier() const { return identifier; }

		virtual Type solveScopes(std::deque<SymbolTable*>* environments, int *varCounter, CFG *cfg, ErrorList &errors) = 0;
		virtual std::string buildIR(CFG* cfg) = 0;

	protected:
		Element(const std::string &id): identifier(id) {}
		std::string identifier;
};


class VarDecl : public Element
{
	public:
		VarDecl(const std::string &id, unsigned int arraySize, Type type = PLACEHOLDER_TYPE) : Element(id), type(type), arraySize(arraySize) {}
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		~VarDecl();

		// only change if currently placeholder
		void updateType(Type type);
		ElementType getType() const{return VAR_DECL;};
		Type getVarType() { return type; }

		Type solveScopes(std::deque<SymbolTable*>* environments, int *varCounter, CFG *cfg, ErrorList &errors);
		std::string buildIR(CFG* cfg);

	protected:
		Type type;
		unsigned int arraySize;

};

class VarDeclList : public Element
{
	protected:
		Type type;
		std::vector<Element *> *declarations;

	public:
		VarDeclList(Type type, std::vector<Element *> *declarations);
		~VarDeclList();
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		ElementType getType() const{ return VAR_DECL; };

		Type solveScopes(std::deque<SymbolTable*>* environments, int *varCounter, CFG *cfg, ErrorList &errors);
		std::string buildIR(CFG* cfg);
};

class VarDef : public Element
{
	public:
		VarDef(VarDecl *decl, Expression *value): Element(decl->getIdentifier()), decl(decl), value(value) {}
		~VarDef();
		void print(GraphPrinter *) const;
		std::string printSelf() const;

		// only change if currently placeholder
		void updateType(Type type);
		ElementType getType() const{return VAR_DEF;};
		Expression* getValue() { return value; }
		Type getVarType() { return decl->getVarType(); }

		Type solveScopes(std::deque<SymbolTable*>* environments, int *varCounter, CFG *cfg, ErrorList &errors);
		std::string buildIR(CFG* cfg);

	protected:
		VarDecl *decl;
		Expression *value;

};

class FuncDecl : public Element
{
	public:
		FuncDecl(const std::string &id, Type type, std::vector<Element *> *args): Element(id), functionType(type), args(args) {}
		~FuncDecl();
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		ElementType getType() const { return FUNC_DECL; }
		std::vector<Element *> *getArgs() { return args;}
		Type getFuncType() { return functionType; }

		Type solveScopes(std::deque<SymbolTable*>* environments, int *varCounter, CFG *cfg, ErrorList &errors);
		std::string buildIR(CFG* cfg);
	protected:
		Type functionType;
		std::vector<Element *> *args;
};


class FuncDef : public Element
{
	public:
		FuncDef(const std::string &id, Type type, std::vector<Element *> *args, Block *b): Element(id), decl(id, type, args), block(b) {}
		~FuncDef();
		void print(GraphPrinter *) const;
		std::string printSelf() const;
		ElementType getType() const { return FUNC_DEF; }

	 	Block*& getBlock()  { return block; }
		FuncDecl *getDecl() { return &decl; }

		Type solveScopes(std::deque<SymbolTable*>*,int *varCounter, CFG *cfg, ErrorList &errors);
		std::string buildIR(CFG* cfg);

	protected:
		FuncDecl decl;
		Block *block;
};


class Document : public Node
{
	public:
		~Document();
		void print(GraphPrinter *) const;
		std::string printSelf() const;

		void addElement(Element *e);

		void createBlocks();

		ElementType getType() const {return DOCUMENT;}


		Type solveScopes(std::deque<SymbolTable*>* environments, int *varCounter, CFG *cfg, ErrorList &errors) { return NOTYPE; }
		std::string buildIR(CFG* cfg);

	std::vector<Element*>& getElements()  {
		return elements;
	}

	protected:
		std::vector<Element *> elements;


};
#endif
