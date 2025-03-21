#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <types.h>
#include <utils.h>

void inverse_image(file_info file_info) {
    header_struct hs;
    info_header ih;

    memcpy(&hs, file_info.file, 14);
    memcpy(&ih, file_info.file + 14, 40);

    printf("size : %d\n", ih.size);
    printf("width : %d\n", ih.width);
    printf("height : %d\n", ih.height);
    printf("bitcount : %d\n", ih.bitcount);
    printf("compression : %d\n", ih.compression);
    printf("imageSize : %d\n", ih.imageSize);
    printf("colorsUsed : %d\n", ih.colorsUsed);
    printf("colorsImportant : %d\n", ih.colorsImportant);

    char* start = file_info.file + 14 + ih.size;
    for (ssize_t i = 0; i < ih.imageSize / 4; i++) {
        pixel_struct* pixel = (pixel_struct*)(&start[i * 4]);
        pixel->r = 255 - pixel->r;
        pixel->g = 255 - pixel->g;
        pixel->b = 255 - pixel->b;
    }
}

int main() {
    file_info image_file = read_file("examples/image.bmp");
    inverse_image(image_file);
    write_file("examples/result.bmp", image_file);

    free(image_file.file);
}
