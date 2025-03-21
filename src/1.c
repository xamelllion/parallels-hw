#include <stddef.h>
#include <stdlib.h>
#include <utils.h>

int main() {
    file_info image_file = read_file("examples/image.bmp");
    write_file("examples/image2.bmp", image_file);

    free(image_file.file);
}
