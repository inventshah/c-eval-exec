// Sachin Shah

#include "eval.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static inline void check_null(void *ptr, char *msg)
{
	if (ptr == NULL)
	{
		fprintf(stderr, "error in eval: %s\n", msg);
		exit(0);
	}
}

void *write_eval(char *content, char*type)
{
	FILE *fp;
	char *kill_char, *copy;
	char filename[9] = "XXXXXX";
	char cmd[40];
	size_t len;
	void *c;

	// Get a temporary file name, and add the .c extension
	mktemp(filename);
	filename[6] = '.';
	filename[7] = 'c';
	filename[8] = '\0';

	// Create the c file
	fp = fopen(filename, "w");
	check_null(fp, "failed to open eval file");

	// set the cmd string to the compile command
	sprintf(cmd, "gcc -shared -fpic -o %s %s", filename, filename);

	// Copy the content to an editable buffer
	len = strlen(content);
	copy = (char *) calloc(sizeof(char), len + 1);
	check_null(copy, "failed to calloc space for content copy");

	strcpy(copy, content);

	if (copy[len - 1] == ';') copy[len - 1] = '\0';
	kill_char = strrchr(copy, ';');

	fprintf(fp, "%s _eval(void){\n", type);
	if (kill_char != NULL)
	{
		kill_char[0] = '\0';
		fprintf(fp, "%s;\nreturn %s;}", copy, kill_char + 1);
	}
	else fprintf(fp, "return %s;}", copy);

	free(copy);

	fclose(fp);

	if (system(cmd)) c = NULL;
	else c = dlopen(filename, RTLD_NOW | RTLD_GLOBAL);

	// Remove the .c file
	sprintf(cmd, "rm -f %s", filename);
	system(cmd);

	return c;
}
