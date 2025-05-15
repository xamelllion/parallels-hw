#include <pct/parallel-conv.h>
#include <pct/serial-conv.h>
#include <pct/utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argparse.h"

void parse_arguments(int argc, const char** argv, struct pct_options* options) {
    const char* mode = NULL;

    struct argparse_option argparse_options[] = {
        OPT_HELP(),
        OPT_STRING('i', "input", &options->read_path, "image to read", NULL, 0, 0),
        OPT_STRING('o', "output", &options->write_path, "path to save result", NULL, 0, 0),
        OPT_STRING('m', "mode", &mode, "mode: seq, pixel, row, column, grid", NULL, 0, 0),
        OPT_INTEGER('t', "threads", &options->threads, "number of threads", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, argparse_options, NULL, 0);
    argparse_describe(&argparse, NULL, NULL);
    argc = argparse_parse(&argparse, argc, argv);

    if (options->read_path == NULL) {
        error("Missing required argument: 'input'\n");
    }

    if (options->write_path == NULL) {
        error("Missing required argument: 'output'\n");
    }

    if (mode != NULL) {
        if (strcmp(mode, "seq") == 0)
            options->mode = seq_mode;
        else if (strcmp(mode, "pixel") == 0)
            options->mode = pixel_mode;
        else if (strcmp(mode, "row") == 0)
            options->mode = row_mode;
        else if (strcmp(mode, "column") == 0)
            options->mode = column_mode;
        else if (strcmp(mode, "grid") == 0)
            options->mode = grid_mode;
        else
            error("Argument 'mode' has unknown value\n");
    } else {
        error("Missing required argument: 'mode'\n");
    }

    if (options->threads == -1) {
        error("Missing required argument: 'threads'\n");
    }
}

int main(int argc, const char** argv) {
    struct pct_options options = {
        .threads = -1,
    };

    parse_arguments(argc, argv, &options);

    if (options.mode == seq_mode) {
        serial_run(options);
    } else {
        parallel_run(options);
    }

    return 0;
}
