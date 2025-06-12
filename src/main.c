#include "argparse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pct/filters.h>
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
        OPT_BOOLEAN('l', "log", &options->log, "write logs", NULL, 0, 0),
        OPT_BOOLEAN('q', "queue", &options->queue, "work with multiple images", NULL, 0, 0),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, argparse_options, NULL, 0);
    argparse_describe(&argparse, "pct - parallel convolution tool", NULL);
    argc = argparse_parse(&argparse, argc, argv);

    // Handle positional arguments for queue mode
    if (options->queue && argc > 0) {
        options->image_paths = malloc(argc * sizeof(char*));
        options->image_count = argc;
        for (int i = 0; i < argc; i++) {
            options->image_paths[i] = malloc(strlen(argv[i]) + 1);
            strcpy(options->image_paths[i], argv[i]);
        }
    } else {
        options->image_paths = NULL;
        options->image_count = 0;
    }

    // Validation for single image mode
    if (!options->queue) {
        if (options->read_path == NULL) {
            error("Missing required argument: 'input'\n");
        }

        if (options->write_path == NULL) {
            error("Missing required argument: 'output'\n");
        }
    } else {
        // Validation for queue mode
        if (options->image_count == 0) {
            error("Queue mode requires at least one image path as positional argument\n");
        }

        if (options->write_path == NULL) {
            error("Missing required argument: 'output' (output directory for queue mode)\n");
        }
    }

    if (filter != NULL) {
        if (strcmp(filter, "id") == 0) {
            options->filter_type = id_filter_type;
        } else if (strcmp(filter, "bl") == 0) {
            options->filter_type = blur_filter_type;
        } else if (strcmp(filter, "mb") == 0) {
            options->filter_type = mb_filter_type;
        } else if (strcmp(filter, "ed") == 0) {
            options->filter_type = edges_filter_type;
        } else if (strcmp(filter, "sr") == 0) {
            options->filter_type = sharpen_filter_type;
        } else
            error("Argument 'filter' has unknown value\n");
    } else {
        error("Missing required argument: 'filter'\n");
    }

    if (mode != NULL) {
        if (strcmp(mode, "seq") == 0) {
            options->mode = seq_mode;
        } else if (strcmp(mode, "pixel") == 0) {
            options->mode = pixel_mode;
        } else if (strcmp(mode, "row") == 0) {
            options->mode = row_mode;
        } else if (strcmp(mode, "column") == 0) {
            options->mode = column_mode;
        } else if (strcmp(mode, "grid") == 0) {
            options->mode = grid_mode;
        } else
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
        .log = 0,
        .queue = 0,
        .image_paths = NULL,
        .image_count = 0,
    };

    parse_arguments(argc, argv, &options);

    struct filter* filters = init_filters();

    if (options.queue) {
        // Process multiple images
        for (int i = 0; i < options.image_count; i++) {
            struct pct_options single_options = options;
            single_options.read_path = options.image_paths[i];

            // Create output filename by combining output directory with input filename
            const char* input_filename = strrchr(options.image_paths[i], '/');
            if (input_filename == NULL) {
                input_filename = options.image_paths[i];
            } else {
                input_filename++;  // Skip the '/'
            }

            char* output_path = malloc(strlen(options.write_path) + strlen(input_filename) + 2);
            sprintf(output_path, "%s/%s", options.write_path, input_filename);
            single_options.write_path = output_path;

            if (options.log) {
                printf("Processing image %d/%d: %s -> %s\n", i + 1, options.image_count,
                       single_options.read_path, single_options.write_path);
            }

            if (options.mode == seq_mode) {
                serial_run(single_options, filters);
            } else {
                parallel_run(single_options, filters);
            }

            free(output_path);
        }

        // Free allocated memory
        for (int i = 0; i < options.image_count; i++) {
            free(options.image_paths[i]);
        }
        free(options.image_paths);
    } else {
        // Process single image
        if (options.mode == seq_mode) {
            serial_run(options, filters);
        } else {
            parallel_run(options, filters);
        }
    }

    free_filters(filters);

    return 0;
}
