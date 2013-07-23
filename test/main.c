#include <stdio.h>
#include <stdlib.h>

#include "Parser.h"

int main(int argc, const char *argv[])
{
	bool   b_test = false;
	int    i_test = 10;
	double d_test = 3.14159;

	Args *args = Args_New();
	Args_Error err;

	Args_Add(args, "-t", NULL, T_BOOL, &b_test, "Un test des booléens.");
	Args_Add(args, "-i", "--int_test", T_INT, &i_test, "Un test des entiers.");
	Args_Add(args, "-d", "--double_test", T_DOUBLE, &d_test, "Un test des doubles.");

	err = Args_Parse(args, argc, argv);
	if( err == TREAT_ERROR )
	{
		Args_Free(args);
		return EXIT_FAILURE;
	}
	else if( err == HELP )
	{
		Args_Free(args);
		return EXIT_SUCCESS;
	}

	printf("%d\n%d\n%g\n", b_test, i_test, d_test);

	CList other = args->rest;

	while(other)
	{
		printf("Untreated options: '%s'\n", other->opt);
		other = other->next;
	}

	Args_Free(args);

	return EXIT_SUCCESS;
}
