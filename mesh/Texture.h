/*
Header file that holds the Texture class
The texture class also finds the coler in the give .bmp file 
*/
#ifndef _texture_h_
#define _texture_h_

#include "../math/Vec3D.h"

class Texture {

    public:
        int width;
        int height;
        unsigned char* data;
        
        Texture(){}
        Texture(const char *file);
        Vec3Df getPixelColor(int i, int j);

};

#endif