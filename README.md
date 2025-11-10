# vex
Cross-platform single-header command line argument parsing library written in C99. It handles many Unix command-line argument conventions, such as grouping together short option names (`-abc`), omiting the space after short option names (`-finput.txt`), halting option parsing with `--`, and handling equals signs after long option names (`--size=1024`).

It also supports higher-level features, such as automatically generating a help dialogue, and grouping together all space-seperated arguments into a single token.

Features not currently supported:
 * Subcommands

## Integration
### Header
All you need to do is include the `vex.h` in your build. Optionally you can include `vex_cpp.hpp`, which provides a C++ wrapper around the C interface. In one (and only one) source file, you will have to define the implementation macros to define all the functionality.
```
#define VEX_IMPLEMENTATION
#include "vex/vex.h"
#undef VEX_IMPLEMENTATION
```

### CMake
This repo is set up in such a way that you can include it as a git submodule, then integrate it into your CMake build with `add_subdirectory`. In this case, it will generate a library file (`libvex.a`) with the function definitions- meaning you won't have to define `VEX_IMPLEMENTATION`, just include the header.

Two options are also provided to control how the library is compiled: 
 * `VEX_BUILD_SHARED` to build as a shared library (defaults to `ON` if `BUILD_SHARED_LIBS` is `ON`, otherwise defaults to `OFF`)
 * `VEX_BUILD_CPP` to build the C++ interface (defaults to `OFF`).
```
set(VEX_BUILD_SHARED OFF) # Build static library
set(VEX_BUILD_CPP ON)     # Build C++ wrapper
add_subdirectory(vex)
```

## Usage
### Basic example
The following is an example of the basic workflow of this library. 

Create a context with `vex_init`:
```
vex_init_info parser_info = {
	.name = "myapp",
	.description = "Your app description here",
	.version = "1.0"
};
vex_ctx parser = vex_init(parser_info);
```
Add arguments to the parser with `vex_add_arg`:
```
/* Valid types:
 * VEX_ARG_TYPE_FLAG: Flag with no further arguments
 * VEX_ARG_TYPE_INT: Integer
 * VEX_ARG_TYPE_DUB: Double
 * VEX_ARG_TYPE_STR: String
 */
vex_arg_desc arg_testflag = {
	.arg_type = VEX_ARG_TYPE_FLAG,
	.long_name = "flag",
	.short_name = 'f',
	.description = "A simple flag"
};
vex_add_arg(&parser, arg_testflag);
```
Parse command line arguments with `vex_parse`:
```
int main(int argc, char** argv) {
	...
	vex_parse(&parser, argc, argv);
}
```
Iterate through tokens with `vex_token_count` and `vex_get_token`:
```
for(int i = 0; i < vex_token_count(&parser); ++i) {
	vex_arg_token* tok = vex_get_token(&parser, i);
	...
}
```
Theres also a shortcut `vex_arg_found` to check if a given argument was provided, good for flags and switches:
```
if (vex_arg_found(&parser, "f")) {
	printf("Flag!"\n);
}
```
When you're done, cleanup with `vex_free`:
```
vex_free(&parser);
```

### Built-in flags
When initializing the library, it automatically adds two flags: `-h / --help` and `-v / --version`. You can retrieve the text generated for these two flags with `vex_get_help` and `vex_get_version` respectively.
```
if (vex_arg_found(&parser, "v")) {
	const char* ver = vex_get_version(&parser);
	printf(ver); // 1.0
	return 0;
}
if (vex_arg_found(&parser, "h")) {
	const char* help = vex_get_help(&parser);
	printf(help); // Usage:... Description:... etc.
	return 0;
}
```
Note that the strings returned by these functions are still owned by the library and should not be freed by the user.

### Multiple arguments per token
When passing a set of arguments like `myapp -i input1.txt input2.txt`, both arguments (`input1.txt` and `input2.txt`) will be grouped together under the same token associated with the `-i` argument. 

The number of arguments under a token is stored in the `arg_count` property. The actual values are stored in `args`, an array of unions representing possible value types. The type of arguments for this token (and therefore which union member to access) is stored in the `arg_type` property.
```
// Iterate through all tokens
for(int i = 0; i < vex_token_count(&parser); ++i) {
	vex_arg_token* tok = vex_get_token(&parser, i);
	
	// If this token is for the 'i' flag,
	if (tok->short_name == 'i') {
		// Iterate through all arguments grouped under it
		for(int j = 0; j < tok->arg_count; ++j) {
			// We know 'i' takes string-type arguments
			printf("Arg %d: %s\n", j, tok->arg[j].str_arg);
		}
	}
}
// Will print:
// 0: input1.txt
// 1: input2.txt
```

### Error handling
Most function will return a bool that indicates if the action was successful. The context object also has a `status` property that can be checked, as well as an `error_msg` property containing a more detailed error string.

Status codes:
 * `VEX_STATUS_OK`: No error
 * `VEX_STATUS_BAD_ALLOC`: Memory allocation failure
 * `VEX_STATUS_BAD_VALUE`: Invalid parameter provided to function
 * `VEX_STATUS_UNKNOWN_ARG`: Unknown flag passed on command line

### Memory allocation
In general, the library will manage its own memory. You dont need to pre-allocate any buffers for it, nor free any pointers it gives you. You only need to run the `vex_free` function when you're done and it will garbage collect.

The library provides hooks to allow for custom memory allocators. These come in the form of macros you define before including the header.
```
#define VEX_MALLOC custom_malloc
#define VEX_REALLOC custom_realloc
#define VEX_FREE custom_free
#include "vex/vex.h"
```