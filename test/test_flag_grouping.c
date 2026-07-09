#include "vex/vex.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
	// Set up context
	vex_init_info parser_info = {
		.name = "test_init",
		.description = "Test the initialization of the vex context.",
		.version = "1.0.0",
		.disable_default_help_arg = false,
		.disable_default_version_arg = false
	};
	vex_ctx parser = { 0 };
	if (!vex_init(&parser, parser_info)) {
		fprintf(stderr, "vex_init() error %d: %s\n", parser.status, parser.status_msg);
		return parser.status;
	}

	// Add flags
	vex_arg_desc arg_testflag = {
		.arg_type = VEX_ARG_TYPE_FLAG,
		.long_name = "flag",
		.short_name = 'f',
		.description = "A simple flag",
		.max_count = 0
	};
	if (!vex_add_arg(&parser, arg_testflag)) {
		fprintf(stderr, "vex_add_arg() error %d: %s\n", parser.status, parser.status_msg);
		return parser.status;
	}
	vex_arg_desc arg_input = {
		.arg_type = VEX_ARG_TYPE_STR,
		.long_name = "input",
		.short_name = 'i',
		.description = "Input files",
		.max_count = 2
	};
	if (!vex_add_arg(&parser, arg_input)) {
		fprintf(stderr, "vex_add_arg() error %d: %s\n", parser.status, parser.status_msg);
		return parser.status;
	}

	// Parse
	if (!vex_parse(&parser, argc, argv)) {
		fprintf(stderr, "vex_parse() error %d: %s\n", parser.status, parser.status_msg);
		return parser.status;
	}

	// Read input files
	for(int i = 0; i < vex_token_count(&parser); ++i) {
		vex_arg_token* tok = vex_get_token(&parser, i);
		if (tok->short_name == 'i') {
			if (tok->arg_type != VEX_ARG_TYPE_STR) {
				fprintf(stderr, "argument type error %d\n", tok->arg_type);
				return -1;
			}
			if (tok->arg_count != 2) {
				fprintf(stderr, "incorrect number of arguments %d\n", tok->arg_count);
				return -1;
			}
			for(int j = 0; j < tok->arg_count; ++j) {
				printf("Arg %d: %s\n", j, tok->arg[j].str_arg);
			}
		}
	}

	// Cleanup
	vex_free(&parser);
	return 0;
}