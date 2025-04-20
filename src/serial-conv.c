#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

void convolution(unsigned char* pixel_array, int w, int h) {
    double factor = 1.0 / 13.0;
    double bias = 0.0;
    unsigned char* result = malloc(w * h * 3);
    int filterWidth = 5;
    int filterHeight = 5;
    double filter[5][5] = {
        0, 0, 1, 0, 0,  //
        0, 1, 1, 1, 0,  //
        1, 1, 1, 1, 1,  //
        0, 1, 1, 1, 0,  //
        0, 0, 1, 0, 0,  //
    };

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            double red = 0.0, green = 0.0, blue = 0.0;
            // multiply every value of the filter with corresponding image pixel
            for (int filterY = 0; filterY < filterHeight; filterY++) {
                for (int filterX = 0; filterX < filterWidth; filterX++) {
                    int imageX = (x - filterWidth / 2 + filterX + w) % w;
                    int imageY = (y - filterHeight / 2 + filterY + h) % h;
                    red += pixel_array[(imageY * w + imageX) * 3 + 0] * filter[filterY][filterX];
                    green += pixel_array[(imageY * w + imageX) * 3 + 1] * filter[filterY][filterX];
                    blue += pixel_array[(imageY * w + imageX) * 3 + 2] * filter[filterY][filterX];
                }
            }
            // truncate values smaller than zero and larger than 255
            result[(y * w + x) * 3 + 0] = min(max((int)(factor * red + bias), 0), 255);
            result[(y * w + x) * 3 + 1] = min(max((int)(factor * green + bias), 0), 255);
            result[(y * w + x) * 3 + 2] = min(max((int)(factor * blue + bias), 0), 255);
        }
    }

    memcpy(pixel_array, result, w * h * 3);
}

void serial_run(const char* read_path, const char* write_path) {
    int w, h, n;
    unsigned char* data = stbi_load(read_path, &w, &h, &n, 3);
    printf("%d %d %d\n", w, h, n);

    convolution(data, w, h);
    convolution(data, w, h);
    convolution(data, w, h);
    convolution(data, w, h);
    convolution(data, w, h);

    stbi_write_png(write_path, w, h, 3, data, w * 3);
}
