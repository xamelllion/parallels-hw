#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <filters.h>
#include <serial-conv.h>
#include <stb_image.h>
#include <stb_image_write.h>
#include <stdlib.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

void convolution(unsigned char* pixel_array, int w, int h, struct filter filter) {
    struct pixel {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };

    struct pixel* result = malloc(w * h * 3);
    struct pixel* image = (struct pixel*)pixel_array;

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

    memcpy(pixel_array, result, w * h * 3);
    free(result);
}

void serial_run(const char* read_path, const char* write_path) {
    int w, h, n;
    unsigned char* data = stbi_load(read_path, &w, &h, &n, 3);

    struct filter motion_blur = {
        .factor = 1.0 / 9.0,
        .bias = 0.0,
        .size = 9,
        .filter = copy_filter_matrix(9, motion_blur_filter_matrix),
    };

    convolution(data, w, h, motion_blur);

    stbi_write_png(write_path, w, h, 3, data, w * 3);
    free_filter_matrix(&motion_blur);
}
