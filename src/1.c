#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>
#include <stdio.h>
#include <types.h>

void inverse_image(unsigned char* pixel_array, size_t pixel_count) {
    for (ssize_t i = 0; i < pixel_count; i++) {
        pixel_struct* pixel = (pixel_struct*)(&pixel_array[i * 3]);
        pixel->r = 255 - pixel->r;
        pixel->g = 255 - pixel->g;
        pixel->b = 255 - pixel->b;
    }
}

int main() {
    int w, h, n;
    unsigned char* data = stbi_load("examples/image.bmp", &w, &h, &n, 3);
    printf("%d %d %d\n", w, h, n);

    inverse_image(data, w * h);
    stbi_write_png("examples/result.png", w, h, 3, data, w * 3);
}
