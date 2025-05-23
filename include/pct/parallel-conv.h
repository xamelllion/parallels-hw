#pragma once

#include <pct/filters.h>
#include <pct/utils.h>

/**
 * @brief Execute parallel image processing pipeline
 *
 * Processes images using multiple threads according to the specified options
 * and applies the provided filters in sequence.
 *
 * @param options Configuration parameters for parallel execution
 * @param filters Array of filters to apply (must be initialized)
 *
 * @note The function assumes proper initialization of both options and filters.
 *
 */
void parallel_run(struct pct_options options, const struct filter* filters);
