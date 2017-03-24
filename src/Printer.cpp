#include "Element.h"
#include "Printer.h"

void GraphPrinter::printGraph(Node *tree)
{
	os << "digraph AST {" << std::endl;
	tree->print(this);
	os << "}" << std::endl;
}

void GraphPrinter::makeNode(Node *n)
{
	size_t nodeId = (size_t)n;
	os << "    " << nodeId << " [label=\"" << n->printSelf() << "\"];" << std::endl;
	counter += 1;
}

void GraphPrinter::addConnection(Node *parent, Node *child)
{
	size_t parentId = (size_t)parent;
	size_t childId = (size_t)child;
	os << "    " << parentId << " -> " << childId << ";" << std::endl;
}

