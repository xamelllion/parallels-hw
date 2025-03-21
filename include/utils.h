#include <stddef.h>

typedef struct {
    size_t filesize;
    char* file;
} file_info;

file_info read_file(const char* filename);
void write_file(const char* filename, const file_info file_info);
