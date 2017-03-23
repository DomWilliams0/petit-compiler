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

enum Type { INT32, INT64, CHAR, VOID };
struct Value
{
	Type type;
	union
	{
		int32_t i32;
		int64_t i64;
		char c;
	};

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

	protected:
		Element(const std::string &id): identifier(id) {};
		std::string identifier;
};


class VarDecl : public Element
{
	public:
		VarDecl(const std::string &id, Type type): Element(id), varType(type) {};
		void print() const;
		~VarDecl();

	protected:
		Type varType;

};

class FuncDecl : public Element
{
	public:
		FuncDecl(const std::string &id, Type type): Element(id), functionType(type) {};
		~FuncDecl();
		void print() const;

	protected:
		Type functionType;
		std::vector<VarDecl> args;
};


class FuncDef : public Element
{
	public:
		FuncDef(const std::string &id, Type type, Block *b): Element(id), decl(id, type), block(b) {};
		~FuncDef();
		void print() const;

	protected:
		FuncDecl decl;
		Block *block;
};


class VarDef : public Element
{
	public:
		VarDef(const std::string &id, const Value &val): Element(id), value(val) {};
		virtual ~VarDef();
		void print() const;

	protected:
		Value value;

};

class Document : public Element
{
	public:
		~Document() {}
		void print() const;

	protected:
		std::vector<Element *> elements;

};
#endif
