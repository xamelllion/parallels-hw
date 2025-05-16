#include <stdlib.h>
#include <time.h>

#include <pct/utils.h>

#define rand_in_range(min, max) ((min) + rand() % ((max) - (min) + 1))

struct rgb_image* generate_image(int width, int height) {
    srand(time(NULL));

    struct rgb_image* image = malloc(width * height * sizeof(struct rgb_image));
    for (size_t i = 0; i < width * height; i++) {
        image[i].r = rand_in_range(0, 255);
        image[i].g = rand_in_range(0, 255);
        image[i].b = rand_in_range(0, 255);
    }

    return image;
}
