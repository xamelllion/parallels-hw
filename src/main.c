#include <parallel-conv.h>
#include <serial-conv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argparse.h"

typedef enum {
    SerialMode,
    ParallelMode,
} pwh_mode;

typedef struct {
    char read_path[256];
    char write_path[256];
    pwh_mode mode;
} phw_options;

void required_argument_error_handler(const char* name) {
    printf("Argument '%s' is required!\n", name);
    exit(1);
}

void parse_arguments(int argc, const char** argv, phw_options* options) {
    const char* read_path = NULL;
    const char* write_path = NULL;
    const char* mode = NULL;

    struct argparse_option argparse_options[] = {
        OPT_HELP(),
        OPT_STRING('i', "input", &read_path, "image to read", NULL, 0, 0),
        OPT_STRING('o', "output", &write_path, "path to save result", NULL, 0, 0),
        OPT_STRING('m', "mode", &mode, "[S]erial/[P]arallel mode", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, argparse_options, NULL, 0);
    argparse_describe(&argparse, NULL, NULL);
    argc = argparse_parse(&argparse, argc, argv);

    if (read_path != NULL) {
        strcpy(options->read_path, read_path);
    } else {
        required_argument_error_handler("input");
    }

    if (write_path != NULL) {
        strcpy(options->write_path, write_path);
    } else {
        required_argument_error_handler("output");
    }

    if (mode != NULL) {
        if (strcmp(mode, "S") == 0)
            options->mode = SerialMode;
        else if (strcmp(mode, "P") == 0)
            options->mode = ParallelMode;
        else {
            printf("Argument 'mode' must have value S or P\n");
            exit(1);
        }
    } else {
        required_argument_error_handler("mode");
    }
}

int main(int argc, const char** argv) {
    phw_options options = {.read_path = "", .write_path = ""};
    parse_arguments(argc, argv, &options);

    if (options.mode == SerialMode) {
        serial_run(options.read_path, options.write_path);
    } else if (options.mode == ParallelMode) {
        parallel_run(options.read_path, options.write_path);
    }

    return 0;
}
