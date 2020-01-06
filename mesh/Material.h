#ifndef _material_h_
#define _material_h_

#include <vector>
#include "../math/Vec3D.h"

class Material {

    public:
        float Ns;
        float Sh;
        Vec3Df Ka;
        Vec3Df Kd;
        Vec3Df Ks;
        Material(const char *path);
        Material(){};
};


#endif