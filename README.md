# Eval for C
Eval and exec like functionality in c using dynamic libraries. Currently only supports POSIX systems with `dlfcn.h`.

### Install:

Generate library binary:

```
cd build
cmake ..
make
```

Run test cases:

```
cd bin
./tests
```

Link the `libceval.a` library to your c file and include the `eval.h` header in the source code. The library and header can be installed globally on your system with `make install` in the build directory.

### Eval Usage:
The basic syntax is `EVAL(code string, variable name, type)`


```
int k = 0;
EVAL("3 + 5", k, int);
```
An ending semicolon is optional. EVAL will automatically set the variable name to the last statement in the code string, however it does not parse the string to find the correct last statement. Put a semicolon before the last statement to ensure the correct value is returned.

```
int sum = 0;
EVAL("int c = 0; for (int i = 1; i < 11; i++) { c += i; } ;c", sum, int);
```

### How it Works:
`EVAL` is a macro that writes, compiles, and loads a dynamic library. Here is an unraveled version of the macro:

```
void *ctr = write_eval(code string, type as a string);
variable = ((type(*)(void)) dlsym(ctr, "foo"))();
dlclose(ctr);
```

`write_eval` will create the dynamic library and return a pointer to it if it was successfully loaded. The macro then calls the wrapper function foo and assigns the variable to the return value.


### Built With:
* C
* POSIX
* Cmake