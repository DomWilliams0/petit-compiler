#include <iostream>
#include "Printer.h"
#include "Element.h"
#include "Expression.h"
#include "Statement.h"
#include <fstream>
#include "Interpreter.h"

extern int yyparse(Document *);

int main(int argc, char **argv)
{
	VarDef* a = new VarDef(new  VarDecl("a", 1), new ConstInteger(0));a->updateType(INT64);
	Affectation * v = new Affectation(new Variable("b"),new Variable("a"));
	VarDef* d = new VarDef(new  VarDecl("d", 1), new ConstInteger(1)); d->updateType(INT64);
	AffectationIncrement* i = new AffectationIncrement(POST_INC, new Variable("d"));

	Block* b = new Block(); 
	std::vector<Node*>* t = new std::vector<Node*>;
	t->push_back(v);
	t->push_back(d);
	t->push_back(i);
	b->setContents(t);

	VarDecl *j = new  VarDecl("b", 1); j->updateType(INT64);
	VarDecl *k = new  VarDecl("c", 1); k->updateType(INT64);
	std::vector<Element*>* args = new std::vector<Element*>;
	args->push_back(j); args->push_back(k);
	FuncDef* F1 = new FuncDef("F1",VOID, args, b);
	VarDef* q = new VarDef(new  VarDecl("c", 1), new ConstInteger(2)); q->updateType(INT64);
	VarDef* l = new VarDef(new  VarDecl("b", 1), new ConstInteger(0)); l->updateType(INT64);

	FunctionAppel* call = new FunctionAppel("F1");
	call->addArg(new Variable("a"));
	call->addArg(new Variable("c"));
	Block* bl = new Block();
	std::vector<Node*>* lb = new std::vector<Node*>;

	lb->push_back(call);
	bl->setContents(lb);
	Cond* cond = new Cond(bl, new BinaryExpression(new Variable("b"), EQ, new ConstInteger(0)));
	Block* m = new Block();
	std::vector<Node*>* kl= new std::vector<Node*>;
	kl->push_back(l);
	kl->push_back(cond);
	m->setContents(kl);
	FuncDef* MAIN = new FuncDef("main", VOID, new std::vector<Element*>, m);

	Document doc;
	doc.addElement(a);
	doc.addElement(F1);
	doc.addElement(q);
	doc.addElement(MAIN);
	
	doc.createBlocks();
	std::ofstream myfile;
	myfile.open("graph.txt");
		
	GraphPrinter printer(myfile);
	printer.printGraph(&doc);
	myfile.close();

	ErrorList errors;
	Interpreter inter(&doc);
	inter.solveScopes(errors);
	bool errors_present = errors.errors.size() > 0;
	if (errors_present)
	{
		std::cerr << errors.errors.size() << " semantic error(s):" << std::endl;
		for (Error &e : errors.errors)
		{
			std::cerr << e.msg << std::endl;
		}
	}

	
	return 0;
}


/*Document d;
int ret = yyparse(&d);
if (ret != 0)
return 1;

d.createBlocks();

GraphPrinter printer(std::cerr);
printer.printGraph(&d);

std::cout << std::endl;*/