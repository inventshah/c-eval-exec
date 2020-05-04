#include "eval.h"
#include "exec.h"

#include <stdio.h>
#include <string.h>

#define NUM_CASES 8

void print_title(int n)
{
	printf("Test case #%d: ", n);
}

void success(void)
{
	printf("success\n");
}
void failed(void)
{
	printf("failed\n");
}

short t1(void)
{
	int k = 5;

	EVAL("3 + 5", k, int);

	return k == (3 + 5);
}
short t2(void)
{
	int k = 5;

	EVAL("16 + 4;", k, int);

	return k == (16 + 4);
}
short t3(void)
{
	int j = 0;

	EVAL("int c = 0; for (int i = 1; i < 11; i++) c+=i; c;", j, int);

	int c = 0; for (int i = 1; i < 11; i++) c+=i;

	return j == c;
}
short t4(void)
{
	int j = 100;
	int k = 0; if (k == 0) { k--; } else if (k == 1) { k++; } k *= 5;

	EVAL("int k = 0; if (k == 0) { k--; } else if (k == 1) { k++; } k *= 5; k;", j, int);

	return j == k;
}
short t5(void)
{
	char j = '1';

	EVAL("#include <string.h>\nchar hello[10] = \"hello\"; hello[0];", j, char);

	return j == 'h';
}
short t6(void)
{
	int q = 1;

	EVAL("3", q, int);

	return q == 3;
}
short t7(void)
{
	int q = 1;

	EVAL("if (5 > 4) {int k = 0; };3", q, int);

	return q == 3;
}
short t8(void)
{
	char *str = "maybe";

	EVAL("\"yes\";", str, char*);

	return strcmp("yes", str) == 0;
}

int main(void)
{
	int i;
	short(* tests[NUM_CASES])(void) = {t1, t2, t3, t4, t5, t6, t7, t8};

	printf("EVAL TESTS:\n");
	for (i = 1; i <= NUM_CASES; i++)
	{
		print_title(i);
		tests[i - 1]() ? success() : failed();
	}

	printf("EXEC TESTS:\n");
	open_exec();
	ADD_VAR(int, x);
	add_var("int", "y");
	add_function("int add(int a, int b)", "return a + b;");
	add_to_scope("#include <stdio.h>");
	printf("status: %d\n", exec("x = add(1, 2); y = add(2, 3); printf(\"%d + %d = %d\\n\", x, y, add(x, y));"));
	close_exec();

	return 0;
}
