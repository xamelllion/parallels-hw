#include <omp.h>
#include <pct/filters.h>
#include <pct/parallel-conv.h>
#include <pct/pthreads-conv.h>
#include <stb_image.h>
#include <stb_image_write.h>
#include <stdlib.h>
#include <string.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

// void parallel_convolution1(unsigned char* pixel_array, int w, int h, struct filter filter) {
//     struct pixel {
//         unsigned char r;
//         unsigned char g;
//         unsigned char b;
//     };

//     struct pixel* result = malloc(w * h * 3);
//     struct pixel* image = (struct pixel*)pixel_array;

// #pragma omp parallel for collapse(2) schedule(dynamic)
//     for (int x = 0; x < w; x++) {
//         for (int y = 0; y < h; y++) {
//             double red = 0.0, green = 0.0, blue = 0.0;

//             for (int filterY = 0; filterY < filter.size; filterY++) {
//                 for (int filterX = 0; filterX < filter.size; filterX++) {
//                     int imageX = (x - filter.size / 2 + filterX + w) % w;
//                     int imageY = (y - filter.size / 2 + filterY + h) % h;

//                     const double coeff = filter.filter[filterY][filterX];

//                     red += image[imageY * w + imageX].r * coeff;
//                     green += image[imageY * w + imageX].g * coeff;
//                     blue += image[imageY * w + imageX].b * coeff;
//                 }
//             }

//             result[(y * w + x)].r = min(max((int)(filter.factor * red + filter.bias), 0), 255);
//             result[(y * w + x)].g = min(max((int)(filter.factor * green + filter.bias), 0), 255);
//             result[(y * w + x)].b = min(max((int)(filter.factor * blue + filter.bias), 0), 255);
//         }
//     }

//     memcpy(pixel_array, result, w * h * 3);
//     free(result);
// }

void parallel_run(const char* read_path, const char* write_path) {
    int w, h, n;
    unsigned char* data = stbi_load(read_path, &w, &h, &n, 3);
    printf("%d %d\n", w, h);

    struct filter motion_blur = {
        .factor = 1.0 / 9.0,
        .bias = 0.0,
        .size = 9,
        .filter = copy_filter_matrix(9, motion_blur_filter_matrix),
    };

    row_convolution(data, w, h, motion_blur, 8);

    stbi_write_png(write_path, w, h, 3, data, w * 3);
    free_filter_matrix(&motion_blur);
}
