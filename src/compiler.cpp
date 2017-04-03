#include <iostream>
#include "Printer.h"
#include "Element.h"

extern int yyparse(Document *);
extern int errorCount;

int main(int argc, char **argv)
{
	Document d;
	int ret = yyparse(&d);
	if (errorCount > 0)
	{
	  std::cerr << errorCount << " errors" << std::endl;
	  return 1;
  }

	if (ret != 0)
		return 2;

	d.createBlocks();

	GraphPrinter printer(std::cerr);
	printer.printGraph(&d);

	std::cout << std::endl;
}
