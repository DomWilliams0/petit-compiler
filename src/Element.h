/*
 * Element.h
 *
 *  Created on: 22 mars 2017
 *      Author: lboucaud
 */

#ifndef ELEMENT_H_
#define ELEMENT_H_
#include <string>
#include <stdint.h>
#include <iostream>
#include <vector>

class Block;

enum Type {INT32,INT64,CHAR,VOID};
struct Value {
	Type type;
	union {
		int32_t i32;
		int64_t i64;
		char c;
	};
	void printValue()
	{
		switch(type)
		{
			case INT32:
				std::cout<<i32<<std::endl;
			break;
			case INT64:
				std::cout<<i64<<std::endl;
			break;
			case CHAR:
				std::cout<<c<<std::endl;
			break;
		}
	}
	void printType()
		{
			switch(type)
			{
				case INT32:
					std::cout<<"int32_t "<<std::endl;
				break;
				case INT64:
					std::cout<<"int64_t "<<std::endl;
				break;
				case CHAR:
					std::cout<<"char "<<std::endl;
				break;
				case VOID:
					std::cout<<"void "<<std::endl;
				break;
			}
		}
};



class Element {

public:

	virtual ~Element();
protected:
	Element(std::string id):identifier(id){};
	std::string identifier;
};


class VarDecl : public Element
{
public:
	VarDecl(std::string id,Type type):Element(id),varType(type){};
	virtual ~VarDecl();
	void print();
protected:
	Type varType;

};

class FuncDecl : public Element
{
public:
	FuncDecl(std::string id,Type type):Element(id),functionType(type){};
	virtual ~FuncDecl();
	void print();
protected:
	Type functionType;
	std::vector<VarDecl> args;
};


class FuncDef : public Element
{
public:
	FuncDef(std::string id,Type type,Block* b):Element(id),functionType(type),block(b){};
	virtual ~FuncDef();
	void print();

protected:
	Type functionType;
	std::vector<VarDecl> args;
	Block* block;
};




class VarDef : public Element
{
public:
	VarDef(std::string id,Value val):Element(id),value(val){};
	virtual ~VarDef();
	void print();
protected:
	Value value;

};

class Document {
public:
	Document();
	virtual ~Document();
	void print();
protected:
	std::vector<Element> elements;

};
#endif /* ELEMENT_H_ */
