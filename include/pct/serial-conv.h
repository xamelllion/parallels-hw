#pragma once

#include <pct/utils.h>

/**
 * @brief Execute serial (single-threaded) image processing pipeline
 *
 * Processes images sequentially on the calling thread according to the specified
 * options and applies the provided filters in order.
 *
 * @param options Configuration parameters for processing (must be valid)
 * @param filters Array of initialized filters to apply sequentially
 *
 * @note The function assumes proper initialization of both options and filters.
 *
 */
void serial_run(const struct pct_options options, const struct filter* filters);
