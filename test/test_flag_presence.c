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

	// Add flag
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

	// Parse
	if (!vex_parse(&parser, argc, argv)) {
		fprintf(stderr, "vex_parse() error %d: %s\n", parser.status, parser.status_msg);
		return parser.status;
	}

	// Check for flag
	if (!vex_arg_found(&parser, "f")) {
		fprintf(stderr, "test flag not found");
		return -1;
	}

	// Cleanup
	vex_free(&parser);
	return 0;
}