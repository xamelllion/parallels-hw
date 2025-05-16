#pragma once

struct rgb_image* generate_image(int width, int height);

int compare_images(struct rgb_image* expected, struct rgb_image* actual, int width, int height);

struct filter filter_composition(const struct filter* f1, const struct filter* f2);

struct filter generate_random_filter(int size);

void free_filter(struct filter* filter);
