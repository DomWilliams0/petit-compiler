#include <stdio.h>
#include "Element.h"
#include "Statement.h"
#include "Expression.h"
#include "c.tab.h"

extern void yy_scan_string(const char *);
extern int yyparse(Document *doc);

bool test_errors_syntax()
{
	Document doc;
	int ret;

	// empty block
	// yy_scan_string("void main() { }");
	// ret = yyparse(&doc);

	return true;
}

int main()
{
	bool pass;
	unsigned int fail_count = 0;

	pass = test_errors_syntax();
	if (!pass)
	{
		std::cerr << (pass ? "PASS" : "FAIL") << std::endl;
		fail_count += 1;
	}

	//test_errors_semantic();
	//test_asm();

	return fail_count == 0 ? 0 : 1;
}
