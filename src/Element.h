#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
#include <stdint.h>
#include <iostream>
#include <vector>

class Block;

enum Type {INT32, INT64, CHAR, VOID};
struct Value
{
	Type type;
	union
	{
		int32_t i32;
		int64_t i64;
		char c;
	};

	void printValue()
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


class Element
{
	public:
		virtual ~Element();

	protected:
		Element(std::string id): identifier(id) {};
		std::string identifier;
};


class VarDecl : public Element
{
	public:
		VarDecl(std::string id, Type type): Element(id), varType(type) {};
		void print();
		~VarDecl();
	protected:
		Type varType;

};

class FuncDecl : public Element
{
	public:
		FuncDecl(std::string id, Type type): Element(id), functionType(type) {};
		~FuncDecl();
		void print();
	protected:
		Type functionType;
		std::vector<VarDecl> args;
};


class FuncDef : public Element
{
	public:
		FuncDef(std::string id, Type type, Block *b): Element(id), functionType(type), block(b) {};
		~FuncDef();
		void print();

	protected:
		Type functionType;
		std::vector<VarDecl> args;
		Block *block;
};




class VarDef : public Element
{
	public:
		VarDef(std::string id, Value val): Element(id), value(val) {};
		virtual ~VarDef();
		void print();
	protected:
		Value value;

};

class Document
{
	public:
		~Document();

		void print();
	protected:
		std::vector<Element *> elements;

};
#endif
