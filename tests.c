#include "eval.h"

#include <stdio.h>
#include <string.h>

#define NUM_CASES 7

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

int main(void)
{
	int i;
	short(* tests[NUM_CASES])(void) = {t1, t2, t3, t4, t5, t6, t7};

	for (i = 1; i <= NUM_CASES; i++)
	{
		print_title(i);
		tests[i - 1]() ? success() : failed();
	}
}
