#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <pct/filters.h>
#include <pct/utils.h>
#include <stb_image.h>
#include <stb_image_write.h>

struct image_info load_image(const char* read_path) {
    struct image_info info;
    info.image = (struct rgb_image*)stbi_load(read_path, &info.width, &info.height, NULL, 3);

    return info;
}

void dump_image(const char* write_path, struct image_info info) {
    stbi_write_png(write_path, info.width, info.height, 3, info.image, info.width * 3);
}
