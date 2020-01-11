/*
Header file that holds the classes Image and RGB
*/

#ifndef _image_h_
#define _image_h_

#include <vector>

class RGB {

    public:
        float r, g, b;

        RGB(float r, float g, float b);

};

class Image {

    public:
        int width, height;
        std::vector<float> image;
        Image(int width, int height);
        void setPixel(int i, int j, RGB rgb);
        bool writeImage(const char * fileName);
};

#endif