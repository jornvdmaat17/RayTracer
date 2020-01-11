
#include "Image.h"
#include <algorithm>
#include <stdlib.h>
#include <iostream>
#include <fstream>

RGB::RGB(float r, float g, float b){
    this->r = std::min(r, 1.0f);
    this->g = std::min(g, 1.0f);
    this->b = std::min(b, 1.0f);
}

Image::Image(int width, int height) : width(width), height(height){
    image.resize(3*width*height);
}

void Image::setPixel(int i, int j, RGB rgb){
    image[3 * (width * j + i)] = rgb.r;
    image[3 * (width * j + i) + 1] = rgb.g;
    image[3 * (width * j + i) + 2] = rgb.b;
}

bool Image::writeImage(const char * fileName){
    std::ofstream f;
    f.open(fileName, std::ios::out | std::ios::binary);  
    f << "P6\n" << width << "\n" << height << "\n255\n";
    for(int i = 0; i < 3 * width * height; i++){
        unsigned int c = (unsigned int)(image[i] * 255.f + 0.5f);
        std::cout << c << std::endl;
        f << c << "\n";
    } 
    f.close();
}
