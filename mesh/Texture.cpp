#include "Texture.h"

/*
Constructor of a Texture.
Takes in a file and reads all the data from the bmp file
*/
Texture::Texture(const char *file){
    int i;
    FILE* f = fopen(file, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    width = *(int*)&info[18];
    height = *(int*)&info[22];

    int size = 3 * width * height;
    unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
    fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
    fclose(f);

    for(i = 0; i < size; i += 3){
        unsigned char tmp = data[i];
        data[i] = data[i+2];
        data[i+2] = tmp;
    }

    this->data = data;
}

/*
Returns the rgb color value of a given pixel in the bmp file
*/
Vec3Df Texture::getPixelColor(int i, int j){
    return Vec3Df((int)(data[3 * (i * width + j)]), (int)(data[3 * (i * width + j) + 1]), (int)(data[3 * (i * width + j) + 2]));
}