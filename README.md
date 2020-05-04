# Eval and Exec for C
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

Link the `libevalexec.a` library to your c file and include the `eval.h` and/or `exec.h` headers in the source code. The library and headers can be installed globally on your system with `make install` in the build directory.

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

### Exec Usage:
The exec function mantains a global scope which allows shared variables and functions across exec usages. Call `open_exec()` to initalize the exec file system and `close_exec()` to close it.

Add a variable to the scope:

* `ADD_VAR(type, name)` no quotes needed
* `add_var(type, name)` as strings

Add a function to the scope: `add_function(signtaure, function content)`

Add an arbitrary line to the scope: `add_to_scope(string)`

Execute some piece of code: `exec(code string)`

Examples:

```
open_exec();
ADD_VAR(int, x); // Adding an int x
add_var("int", "y"); // Adding an int y
add_function("int add(int a, int b)", "return a + b;"); // Creating a simple adding function
add_to_scope("#include <stdio.h>"); // include the standard io library
exec("x = add(1, 2); y = add(2, 3); printf(\"%d + %d = %d\\n\", x, y, add(x, y));"); // exec
close_exec();
```

### How it Works:
`EVAL` is a macro that writes, compiles, and loads a dynamic library. Here is an unraveled version of the macro:

```
void *ctr = write_eval(code string, type as a string);
variable = ((type(*)(void)) dlsym(ctr, "_eval"))();
```

`write_eval` will create the dynamic library and return a pointer to it if it was successfully loaded. The macro then calls the wrapper function _eval and assigns the variable to the return value.

Exec preforms the same tasks, but also links against prior exec libraries to retain variable values.

### Built With:
* C
* POSIX
* Cmake