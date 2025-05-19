#include "argparse.h"
#include "pct/filters.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pct/parallel-conv.h>
#include <pct/serial-conv.h>
#include <pct/utils.h>

void parse_arguments(int argc, const char** argv, struct pct_options* options) {
    const char* mode = NULL;
    const char* filter = NULL;

    struct argparse_option argparse_options[] = {
        OPT_HELP(),
        OPT_STRING('i', "input", &options->read_path, "image to read", NULL, 0, 0),
        OPT_STRING('o', "output", &options->write_path, "path to save result", NULL, 0, 0),
        OPT_STRING('f', "filter", &filter, "filter: id, bl, mb, ed, sr", NULL, 0, 0),
        OPT_STRING('m', "mode", &mode, "mode: seq, pixel, row, column, grid", NULL, 0, 0),
        OPT_INTEGER('t', "threads", &options->threads, "number of threads", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, argparse_options, NULL, 0);
    argparse_describe(&argparse, "pct - parallel convolution tool", NULL);
    argc = argparse_parse(&argparse, argc, argv);

    if (options->read_path == NULL) {
        error("Missing required argument: 'input'\n");
    }

    if (options->write_path == NULL) {
        error("Missing required argument: 'output'\n");
    }

    if (filter != NULL) {
        if (strcmp(filter, "id") == 0)
            options->filter_type = id_filter_type;
        else if (strcmp(filter, "bl") == 0)
            options->filter_type = blur_filter_type;
        else if (strcmp(filter, "mb") == 0)
            options->filter_type = mb_filter_type;
        else if (strcmp(filter, "ed") == 0)
            options->filter_type = edges_filter_type;
        else if (strcmp(filter, "sr") == 0)
            options->filter_type = sharpen_filter_type;
        else
            error("Argument 'filter' has unknown value\n");
    } else {
        error("Missing required argument: 'filter'\n");
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
        if (mode != seq_mode) {
            options->threads = get_thread_count();
        }
    }
}

int main(int argc, const char** argv) {
    struct pct_options options = {
        .threads = -1,
    };

    parse_arguments(argc, argv, &options);

    struct filter* filters = init_filters();

    if (options.mode == seq_mode) {
        serial_run(options, filters);
    } else {
        parallel_run(options, filters);
    }

    free_filters(filters);

    return 0;
}
