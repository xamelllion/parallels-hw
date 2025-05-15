#include <pct/filters.h>
#include <pct/serial-conv.h>
#include <pct/utils.h>
#include <stdlib.h>
#include <string.h>

void serial_convolution(struct rgb_image* image, int w, int h, struct filter filter) {
    struct rgb_image* result = malloc(w * h * sizeof(struct rgb_image));

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            double red = 0.0, green = 0.0, blue = 0.0;

            for (int filterY = 0; filterY < filter.size; filterY++) {
                for (int filterX = 0; filterX < filter.size; filterX++) {
                    int imageX = (x - filter.size / 2 + filterX + w) % w;
                    int imageY = (y - filter.size / 2 + filterY + h) % h;

                    red += image[imageY * w + imageX].r * filter.filter[filterY][filterX];
                    green += image[imageY * w + imageX].g * filter.filter[filterY][filterX];
                    blue += image[imageY * w + imageX].b * filter.filter[filterY][filterX];
                }
            }

            result[(y * w + x)].r = min(max((int)(filter.factor * red + filter.bias), 0), 255);
            result[(y * w + x)].g = min(max((int)(filter.factor * green + filter.bias), 0), 255);
            result[(y * w + x)].b = min(max((int)(filter.factor * blue + filter.bias), 0), 255);
        }
    }

    memcpy(image, result, w * h * 3);
    free(result);
}

void serial_run(const struct pct_options options) {
    struct image_info info = load_image(options.read_path);

    struct filter motion_blur = {
        .factor = 1.0 / 9.0,
        .bias = 0.0,
        .size = 9,
        .filter = copy_filter_matrix(9, motion_blur_filter_matrix),
    };

    serial_convolution(info.image, info.width, info.height, motion_blur);

    dump_image(options.write_path, info);
    free_filter_matrix(&motion_blur);
}
