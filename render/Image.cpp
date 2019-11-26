
#include "Image.h"
#include <algorithm>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

RGB::RGB(float r, float g, float b){
    r = std::min(r, 1.0f);
    g = std::min(g, 1.0f);
    b = std::min(b, 1.0f);
}

Image::Image(int width, int height) : width(width), height(height){
    image.resize(3*width*height);
}

void Image::setPixel(int i, int j, const RGB & rgb){
    image[3 * (width * j + i)] = rgb.r;
    image[3 * (width * j + i) + 1] = rgb.g;
    image[3 * (width * j + i) + 2] = rgb.b;
}

bool Image::writeImage(const char * fileName){
    std::vector<unsigned char> wImage(image.size());
    for(unsigned int i = 0; i < image.size(); ++i){
        wImage[i] = (unsigned char)(image[i] * 255.0f);
    }

    return stbi_write_bmp(fileName, width, height, 3, &wImage[0]);
}