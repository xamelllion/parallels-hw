#include <stdlib.h>
#include <string.h>

#include <pct/filters.h>
#include <pct/serial-conv.h>
#include <pct/utils.h>

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

void serial_run(const struct pct_options options, const struct filter* filters) {
    struct image_info info = load_image(options.read_path);

    serial_convolution(info.image, info.width, info.height, filters[options.filter_type]);

    dump_image(options.write_path, info);
}
