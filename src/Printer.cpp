#include "Element.h"
#include "Printer.h"

void GraphPrinter::printGraph(Node *tree)
{
	os << "digraph AST {" << std::endl;
	os << "    node [shape=box]" << std::endl;
	tree->print(this);
	os << "}" << std::endl;
}

void GraphPrinter::createNode(const char *prefix, size_t id, const std::string &label)
{
	os << "    " << prefix << id << " [label=\"" << label << "\"];" << std::endl;
}

void GraphPrinter::makeNode(Node *n)
{
	createNode("", (size_t)n, n->printSelf());
}

void GraphPrinter::addConnection(Node *parent, Node *child)
{
	size_t parentId = (size_t)parent;
	size_t childId = (size_t)child;
	os << "    " << parentId << " -> " << childId << ";" << std::endl;
}

void GraphPrinter::addNullConnection(Node *parent, const char *label)
{
	createNode("null", nullCounter, label);

	// add connection
	os << "    " << (size_t)parent << " -> null" << nullCounter  << ";" << std::endl;

	nullCounter += 1;
}
