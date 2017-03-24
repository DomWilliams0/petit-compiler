#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>

class Node;

class GraphPrinter
{
	public:
		GraphPrinter(std::ostream &stream) : os(stream), counter(0) {}
		void printGraph(Node *tree);

		void makeNode(Node *n);
		void addConnection(Node *parent, Node *child);

	protected:
		std::ostream &os;
		unsigned int counter;
};


#endif
