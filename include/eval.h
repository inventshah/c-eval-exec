// Sachin Shah

#ifndef _EVAL_H
#define _EVAL_H

#include <dlfcn.h>

#define EVAL(content, var, type) var=(ctr=write_eval(content, #type))?((type(*)(void))dlsym(ctr,"_eval"))():0

void *ctr;

void *write_eval(char *content, char*type);

#endif