/*

Material header file which holds all the different variables for diffuse, shading and reflection.

*/

#ifndef _material_h_
#define _material_h_

#include <vector>
#include "../math/Vec3D.h"

class Material {

    public:
        float Ns;
        float Sh;
        float Rs;
        Vec3Df Ka;
        Vec3Df Kd;
        Vec3Df Ks;
        Material(const char *path);
        Material(){};
};


#endif