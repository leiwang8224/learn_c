#include <stdio.h>
#include "dbg.h"
#include <dlfcn.h>

typedef int (*lib_function) (const char *data);

int main(int argc, char *argv[]) {
	int rc = 0;
	check (argc == 4, "USAGE: simple_test simple_test_lib.so function data");

	char *lib_file = argv[1];
	char *func_to_run = argv[2];
	char *data = argv[3];

	// use dlopen function to load up the lib that's indicated by lib_file.
	// this function returns a handle that we use later, which works alot like
	// opening a file.
	void *lib = dlopen(lib_file, RTLD_NOW);
	// notice at the end there is dlerror to find out what the library related
	// error was.
	check(lib != NULL, "Failed to open the library %s: %s", lib_file, dlerror());

	// dlsym is used to get a function out of the lib by its string name in
	// func_to_run. This is the powerful part, since I'm dynamically getting a 
	// pointer to a function based on a string I got from the command line argv.
	lib_function func = dlsym(lib, func_to_run);
	check(func != NULL, "Did not find %s function in the library %s: %s", func_to_run, lib_file, dlerror());
	rc = func(data);
	check(rc == 0, "Function %s return %d for data: %s", func_to_run, rc, data);
	// finally close the lib as if it's a file.
	rc = dlclose(lib);
	check(rc == 0, "Failed to close %s", lib_file);
	return 0;

error:
	return 1;
}
