/*
Header file that holds the classes Image and RGB
*/

#ifndef _image_h_
#define _image_h_

#include "../Defines.h"
#include <vector>

/*Class that holds the color*/
class RGB {

    public:
        float r, g, b;

        RGB(float r, float g, float b);

};

/*Class that generates image from pixels*/
class Image {

    public:
        Image();
        void setPixel(int i, int j, RGB rgb);
        bool writeImage(const char * fileName);
    private:
        int width, height;
        unsigned char image[HEIGHT][WIDTH][RGBSIZE] = {0};
        void generateBitmapImage(unsigned char *image, const char* imageFileName);
        unsigned char* createFileHeader(int paddingSize);
        unsigned char* createInfoHeader();
};

#endif