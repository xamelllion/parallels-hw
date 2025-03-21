typedef struct {
    short signature;
    int fileSize;
    int reserved;
    int dataOffset;
} header_struct;

typedef struct {
    int size;
    int width;
    int height;
    short planes;
    short bitcount;
    int compression;
    int imageSize;
    int XpixelsPerM;
    int YpixelsPerM;
    int colorsUsed;
    int colorsImportant;
} info_header;

typedef struct {
    char b;
    char g;
    char r;
    char a;
} pixel_struct;
