#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
#include <stdint.h>
#include <iostream>
#include <vector>

class Node
{
	public:
		virtual ~Node() {}
		virtual void print() const = 0;

	protected:
		Node();
};

class Block;
class Expression;

//                                    vvvvvvvvvvvvvvvv unknown at time of parsing
enum Type { INT32, INT64, CHAR, VOID, PLACEHOLDER_TYPE };
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


void printType(Type type);

class Element : public Node
{
	public:
		virtual ~Element();
		virtual void print() const = 0;

		const std::string &getIdentifier() const { return identifier; }

	protected:
		Element(const std::string &id): identifier(id) {};
		std::string identifier;
};


class VarDecl : public Element
{
	public:
		VarDecl(const std::string &id, unsigned int arraySize, Type type = PLACEHOLDER_TYPE) : Element(id), type(type), arraySize(arraySize) {}
		void print() const;
		~VarDecl();

		// only change if currently placeholder
		void updateType(Type type);

	protected:
		Type type;
		unsigned int arraySize;

};

class VarDeclList : public Node
{
	protected:
		Type type;
		std::vector<VarDecl *> *declarations;

	public:
		VarDeclList(Type type, std::vector<VarDecl *> *declarations) : type(type), declarations(declarations) {}

		void addDeclaration(VarDecl *decl);
};

class VarDef : public Element
{
	public:
		VarDef(VarDecl *decl, Expression *value): Element(decl->getIdentifier()), decl(decl), value(value) {}
		virtual ~VarDef();
		void print() const;

		// only change if currently placeholder
		void updateType(Type type);

	protected:
		VarDecl *decl;
		Expression *value;

};

class FuncDecl : public Element
{
	public:
		FuncDecl(const std::string &id, Type type, std::vector<Element *> *args): Element(id), functionType(type), args(args) {}
		~FuncDecl();
		void print() const;

	protected:
		Type functionType;
		std::vector<Element *> *args;
};


class FuncDef : public Element
{
	public:
		FuncDef(const std::string &id, Type type, std::vector<Element *> *args, Block *b): Element(id), decl(id, type, args), block(b) {}
		~FuncDef();
		void print() const;

	protected:
		FuncDecl decl;
		Block *block;
};


class Document : public Node
{
	public:
		void print() const;

		void addElement(Element *e);

	protected:
		std::vector<Element *> elements;

};
#endif
