#include"Image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Vector3f Image::getValue(const Vector2i &xy) const{
    int x = Clamp(xy[0], 0, size[0] - 1);
    int y = Clamp(xy[1], 0, size[1] - 1);
    int offset = (x + y * size[0]) * channels;
    return Vector3f(data[offset], data[offset + 1], data[offset + 2]);
}

void Image::setValue(const Vector2i &xy, const Vector3f &val){
    int offset = (xy[0] + xy[1] * size[0]) * channels;
    data[offset] = val[0];
    data[offset + 1] = val[1];
    data[offset + 2] = val[2];
}

void Image::saveHDR(const char *filename) const{
    stbi_write_hdr(filename, size[0], size[1], 3, data);
}