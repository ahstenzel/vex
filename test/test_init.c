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
		return 1;
	}

	// Cleanup
	vex_free(&parser);
	return 0;
}