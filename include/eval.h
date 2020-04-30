// Sachin Shah

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define EVAL(content, var, type) var=(ctr=write_eval(content, #type))?((type(*)(void))dlsym(ctr,"foo"))():0;dlclose(ctr)

void *ctr;

void *write_eval(char *content, char*type);