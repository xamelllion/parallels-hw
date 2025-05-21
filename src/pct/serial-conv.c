#include <stdio.h>
#include <stdlib.h>

#include <pct/filters.h>
#include <pct/serial-conv.h>
#include <pct/utils.h>

void serial_convolution(struct image_info* info, struct filter filter) {
    info->result = malloc(info->width * info->height * sizeof(struct rgb_image));

    int w = info->width;
    int h = info->height;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            double red = 0.0, green = 0.0, blue = 0.0;

            for (int filterY = 0; filterY < filter.size; filterY++) {
                for (int filterX = 0; filterX < filter.size; filterX++) {
                    int imageX = (x - filter.size / 2 + filterX + w) % w;
                    int imageY = (y - filter.size / 2 + filterY + h) % h;

                    red += info->image[imageY * w + imageX].r * filter.filter[filterY][filterX];
                    green += info->image[imageY * w + imageX].g * filter.filter[filterY][filterX];
                    blue += info->image[imageY * w + imageX].b * filter.filter[filterY][filterX];
                }
            }

            info->result[(y * w + x)].r =
                min(max((int)(filter.factor * red + filter.bias), 0), 255);
            info->result[(y * w + x)].g =
                min(max((int)(filter.factor * green + filter.bias), 0), 255);
            info->result[(y * w + x)].b =
                min(max((int)(filter.factor * blue + filter.bias), 0), 255);
        }
    }
}

void serial_run(const struct pct_options options, const struct filter* filters) {
    struct image_info info = load_image(options.read_path);

    double start_time = get_time_in_seconds();
    serial_convolution(&info, filters[options.filter_type]);
    double end_time = get_time_in_seconds();

    if (options.log) {
        printf("convolution time: %lf\n", end_time - start_time);
    }

    dump_image(options.write_path, info);
}
