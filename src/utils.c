#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <utils.h>

file_info read_file(const char* filename) {
    FILE* file = fopen(filename, "rb");

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char* buffer = malloc(file_size);
    fread(buffer, 1, file_size, file);
    fclose(file);

    file_info info = {
        .filesize = file_size,
        .file = buffer,
    };

    return info;
}

void write_file(const char* filename, const file_info file_info) {
    FILE* output_file = fopen(filename, "wb");

    fwrite(file_info.file, 1, file_info.filesize, output_file);
    fclose(output_file);
}
