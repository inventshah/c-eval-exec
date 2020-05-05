// Sachin Shah

#include "exec.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Check if a pointer is null
static inline void check_null(void *ptr, char *msg)
{
	if (ptr == NULL)
	{
		fprintf(stderr, "error in exec: %s\n", msg);
		exit(0);
	}
}

// Create a library 1 for success and 0 for error
int create_lib(char *outside, char *inside)
{
	FILE *fp;
	char filename[21] = "_execfiles/XXXXXX";
	char cmd[128];
	void * ctr;

	// Get a temporary file name, and add the .c extension
	mktemp(filename);
	sprintf(filename + 17, ".c");

	// Create the c file
	fp = fopen(filename, "w");
	check_null(fp, "failed to open exec file");

	// add the scope and any outside information
	fprintf(fp, "#include \"scope.h\"\n%s\n", outside);

	// add the exec function and the inner code to execute
	fprintf(fp, "void _exec(void){\n%s;\n}\n", inside);

	fclose(fp);

	// clear the .c extension
	filename[17] = '\0';

	// set the cmd string to the compile command
	sprintf(cmd, "gcc -shared -fpic -o %s.so %s.c _execfiles/*.so", filename, filename);
	if (system(cmd)) // there was an error, remove the file
	{
		sprintf(cmd, "rm -f %s.c", filename);
		system(cmd);
		return 0;
	}
	else
	{
		// get the exec function and call it
		sprintf(filename + 17, ".so");
		ctr = dlopen(filename, RTLD_NOW | RTLD_GLOBAL);
		if (ctr != NULL) ((void(*)(void)) dlsym(ctr, "_exec"))();
		else return 0;
	}

	return 1;
}

// Create the global scope header file
void create_scope(void)
{
	FILE *fp;

	fp = fopen("_execfiles/scope.h", "w");
	check_null(fp, "failed to open scope file");
	fprintf(fp, "\n");
	fclose(fp);
}

// Create an empty library for linking
void create_empty(void)
{
	FILE *fp;

	// Create a null file
	fp = fopen("_execfiles/empty.c", "w");
	check_null(fp, "failed to open empty file");
	fclose(fp);

	// Compile into a shared library
	system("gcc -shared -fpic -o _execfiles/empty.so _execfiles/empty.c");
}

// Create the _execfiles directory and initalize a scope and empty library
void open_exec(void)
{
	system("mkdir _execfiles");
	create_empty();
	create_scope();
}

// remove the execfiles directory
void close_exec(void)
{
	system("rm -rf _execfiles");
}

// Add a function to the scope and create a library with it's content
int add_function(char *signature, char* content)
{
	char *line = NULL;
	char *function = NULL;
	int ret = 0;
	size_t signature_length = strlen(signature);

	// create the scope line with signature;
	line = (char *) calloc(sizeof(char), signature_length + 2);
	check_null(line, "failed to calloc space for a scope function line");
	sprintf(line, "%s;", signature);
	add_to_scope(line);
	free(line);

	// create the function as signature{content}
	function = (char *) calloc(sizeof(char), signature_length + strlen(content) + 3);
	check_null(function, "failed to calloc space for a function");
	sprintf(function, "%s{%s}", signature, content);

	// create the library and free 
	ret = create_lib(function, "");
	free(function);

	return ret;
}

// Add a variable to the scope, add extern and create a library with just the variable
void add_var(char *type, char *name)
{
	char *line = NULL;

	// Create a line with extern ;
	line = (char *) calloc(sizeof(char), strlen(type) + strlen(name) + 10);
	check_null(line, "failed to calloc space for a scope variable line");
	sprintf(line, "extern %s %s;", type, name);
	add_to_scope(line);

	// Create a library with just type name;
	sprintf(line, "%s %s;", type, name);
	create_lib(line, "");
	free(line);
}

// Add an arbitrary line to the scope header file
void add_to_scope(char *content)
{
	FILE *fp;

	fp = fopen("_execfiles/scope.h", "a");
	check_null(fp, "failed to open scope file");
	fprintf(fp, "%s\n", content);
	fclose(fp);
}

int exec(char *content)
{
	return create_lib("", content);
}
