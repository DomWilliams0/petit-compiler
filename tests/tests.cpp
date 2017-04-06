#include <sstream>
#include <algorithm>
#include <stdio.h>
#include "Element.h"
#include "Statement.h"
#include "Expression.h"
#include "c.tab.h"

extern void yy_scan_string(const char *);
extern int yyparse(Document *);
extern unsigned int errorCount;
unsigned int fail_count = 0;

#define ASSERT(what, x, expected) do { \
	std::ostringstream out; \
	bool pass = parse_string(x, out) == expected; \
	std::cerr << what << " ... " << (pass ? "pass" : "fail"); \
	if (!pass) \
	{ \
		fail_count += 1; \
		std::string output(out.str()); \
		std::replace(output.begin(), output.end(), '\n', '|'); \
		std::cerr << " (" << output << ")"; \
	} \
	std::cerr << std::endl; \
	} while (false)

#define ASSERT_FAIL(what, x) ASSERT(what, x, false)
#define ASSERT_PASS(what, x) ASSERT(what, x, true)

bool parse_string(const char *s, std::ostringstream &out)
{
	std::streambuf *original = std::cerr.rdbuf();
	std::cerr.rdbuf(out.rdbuf());

	errorCount = 0;

	Document d;
	yy_scan_string(s);
	bool pass = yyparse(&d) == 0;

	if (pass && errorCount == 0)
	{
		d.createBlocks();

		ErrorList errors;
		Interpreter interpreter(&d);
		interpreter.solveScopes(errors);
		errorCount += errors.errors.size(); // TODO differentiate between lex, syntax and semantic errors

		bool comma = false;
		for (Error &e : errors.errors)
		{
			if (comma) out << ", ";
			comma = true;
			out << e.msg;
		}
	}

	std::cerr.flush();
	std::cerr.rdbuf(original);

	return pass && errorCount == 0;
}

void test_errors_lexer()
{
	ASSERT_FAIL("Bad char", "@");
	ASSERT_FAIL("Bad char in program", "void main() { $ }");
	ASSERT_FAIL("Bad char in identifier", "int32_t ab@#c;");
	ASSERT_FAIL("Identifier starting with number", "int32_t 123abc;");
}

void test_errors_syntax()
{
	ASSERT_FAIL("Two operators", "void main() { 1 * / 2; }");
	ASSERT_FAIL("Two operands", "void main() { 1 2; }");
	ASSERT_FAIL("Missing semicolon", "void main() { 1 + 2 }");
	ASSERT_FAIL("Missing bracket", "void main() { 1 + ( 2 * 3; }");
	ASSERT_FAIL("Too many closing brackets", "void main() { ( 1 + 2 ) ); }");
}

void test_errors_semantic()
{
	ASSERT_FAIL("Missing var declaration", "void main() { a = 5; }");
	ASSERT_FAIL("Missing func declaration", "void main() { a(); }");
	ASSERT_FAIL("Var redeclaration", "char a, a;");

	// forward declarations
	ASSERT_PASS("Func redeclaration with same signatures", "void a(); void a();");
	// ASSERT_FAIL("Func redeclaration with different return types", "void a(); char a();");

	ASSERT_FAIL("Func redefinition", "void a() {} void a() {}");

	ASSERT_FAIL("Bad func args number", "void a(char c, char d) {} void main() { a(); }");
	ASSERT_FAIL("Bad func args type", "void a(char c) {} void main() { a(5); }");

	ASSERT_FAIL("Return value in void function", "void main() { return 5; }");
	ASSERT_FAIL("Return void in non-void function", "int64_t main() { return; }");
	ASSERT_FAIL("Return wrong type", "int64_t main() { return 'a'; }");

	ASSERT_FAIL("Mismatched type in assignment", "void main() { char a = 5; }");
}

void test_valid_programs()
{
	ASSERT_PASS("Global declaration", "int32_t a;");
	ASSERT_PASS("Global declaration and definition", "int64_t a = 5;");
	ASSERT_PASS("Multiple definitions and declarations", "int64_t a, b = 5, c = 2, d, e;");
	ASSERT_PASS("Include", "#include <stdint.h>\n int64_t a, b = 5, c = 2, d, e;");
	ASSERT_PASS("Increment and decrement", "void main() { int64_a a = 5; a++; a--; ++a; --a;");
	// ... etc
}

#define RUN_SUITE(suite) run_suite(suite, #suite)

void run_suite(void (*suite)(), const char *name)
{
	std::cerr << "-=-=-=- " << name << " -=-=-=-" << std::endl;
	fail_count = 0;

	suite();

	std::cerr << "-=-=-=- " << (fail_count == 0 ? "PASS" : "FAIL") << " -=-=-=-" << std::endl << std::endl;
}

int main()
{
	RUN_SUITE(test_errors_lexer);
	RUN_SUITE(test_errors_syntax);
	RUN_SUITE(test_errors_semantic);
	RUN_SUITE(test_valid_programs);
	return 0;
}
