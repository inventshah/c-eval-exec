// Sachin Shah

#include "exec.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static inline void check_null(void *ptr, char *msg)
{
	if (ptr == NULL)
	{
		fprintf(stderr, "error in exec: %s\n", msg);
		exit(0);
	}
}

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

	fprintf(fp, "#include \"scope.h\"\n%s\n", outside);
	fprintf(fp, "void _exec(void){\n%s;\n}\n", inside);

	fclose(fp);

	filename[17] = '\0';

	// set the cmd string to the compile command
	sprintf(cmd, "gcc -shared -fpic -o %s.so %s.c _execfiles/*.so", filename, filename);
	if (system(cmd))
	{
		sprintf(cmd, "rm -f %s", filename);
		system(cmd);
		return 0;
	}
	else
	{
		sprintf(filename + 17, ".so");
		ctr = dlopen(filename, RTLD_NOW | RTLD_GLOBAL);
		if (ctr != NULL) ((void(*)(void)) dlsym(ctr, "_exec"))();
		else return 0;
	}

	return 1;
}

void create_scope(void)
{
	FILE *fp;

	fp = fopen("_execfiles/scope.h", "w");
	check_null(fp, "failed to open scope file");
	fprintf(fp, "\n");
	fclose(fp);
}

void create_empty(void)
{
	FILE *fp;

	fp = fopen("_execfiles/empty.c", "w");
	check_null(fp, "failed to open empty file");
	fclose(fp);

	system("gcc -shared -fpic -o _execfiles/empty.so _execfiles/empty.c");
}

void open_exec(void)
{
	system("mkdir _execfiles");
	create_empty();
	create_scope();
}
void close_exec(void)
{
	system("rm -rf _execfiles");
}

int add_function(char *signature, char* content)
{
	char *line = NULL;
	char *function = NULL;
	size_t signature_length = strlen(signature);

	line = (char *) calloc(sizeof(char), signature_length + 2);
	check_null(line, "failed to calloc space for a scope function line");
	sprintf(line, "%s;", signature);
	add_to_scope(line);

	function = (char *) calloc(sizeof(char), signature_length + strlen(content) + 3);
	check_null(function, "failed to calloc space for a function");
	sprintf(function, "%s{%s}", signature, content);

	return create_lib(function, "");
}

void add_var(char *type, char *name)
{
	char *line = NULL;

	line = (char *) calloc(sizeof(char), strlen(type) + strlen(name) + 10);
	check_null(line, "failed to calloc space for a scope variable line");
	sprintf(line, "extern %s %s;", type, name);
	add_to_scope(line);

	sprintf(line, "%s %s;", type, name);
	create_lib(line, "");
}

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
