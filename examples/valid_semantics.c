#include <stdint.h>
#define IGNORE_ME

void func_decl();

void func_def()
{
}

void do_something(int64_t i)
{
	// do nothing
	char newline = '\m';
}

void main()
{
	// error
	not_declared_func();

	func_def();

	char c = 'a';

	// error
	c = 5;

	for (int64_t i = 0; i < 20; i++)
	{
		if (i % 2 == 0)
		{
			do_something(i + 2);
		}
	}

}
