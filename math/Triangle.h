#ifndef _triangle_h_
#define _triangle_h_

#include <string>
#include "Vec3D.h"

class Triangle {
    
    public:
        Triangle();
        Triangle(const Triangle &t);
        Triangle(int x, int y, int z);
        const std::string toString();
        int v0;
        int v1;
        int v2;

        void setEdges(Vec3Df edge0, Vec3Df edge1);
        Vec3Df edge0;
        Vec3Df edge1;
        ~Triangle();
};

#endif 
