// Sachin Shah

#ifndef _EXEC_H
#define _EXEC_H

#include <dlfcn.h>

#define ADD_VAR(type, name) add_var(#type, #name)

void open_exec(void);
void close_exec(void);

int add_function(char *signature, char* content);
void add_var(char *type, char *name);

void add_to_scope(char *content);

int exec(char *content);

#endif