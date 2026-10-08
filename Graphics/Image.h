#include <stdio.h>
#include <stdlib.h>

#include "../Types.h"
#include "Math.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct { 
    uint width; 
    uint height; 
    uint* pixels;
    vec2 offset; // Image-space rotation pivot in pixels.
    vec2 scale;
    float rotation;
} Image;

Image Image_Load_Png(const char* filename) {
    Image img = {0, 0, NULL, (vec2){0, 0}, (vec2){1, 1}, 0.f};
    int width, height, channels;

    uchar* raw_data = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);

    const uint n = (uint)width*(uint)height;
    uint i = 0;
    img.pixels = (uint*)malloc(n * sizeof(uint));
    while (i < n) {
        uchar *rgba = raw_data + i * 4;
        img.pixels[i] = ((uint)rgba[3] << 24)
                    | ((uint)rgba[0] << 16)
                    | ((uint)rgba[1] << 8)
                    | (uint)rgba[2];
        i++;
    }

    if (raw_data == NULL) {
        printf("Error: Could not load image %s\n", filename);
        return img;
    }

    img.width = (uint)width;
    img.height = (uint)height;

    return img;
}

void Image_Free(Image* img) {
    if (img->pixels != NULL) {
        stbi_image_free(img->pixels);
        img->pixels = NULL;
    }
}