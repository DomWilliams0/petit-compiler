#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>

class Node;

class GraphPrinter
{
	public:
		GraphPrinter(std::ostream &stream) : os(stream), nullCounter(0) {}
		void printGraph(Node *tree);

		void makeNode(Node *n);
		void addConnection(Node *parent, Node *child);
		void addNullConnection(Node *parent, const char *label);

	protected:
		std::ostream &os;
		size_t nullCounter;

		void createNode(const char *prefix, size_t id, const std::string &label);
};


#endif
