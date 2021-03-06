/*
Header file that holds the Triangle class and its functions

The triangle holds indices for vectors, normals and textures.
*/

#ifndef _triangle_h_
#define _triangle_h_

#include <string>
#include "Vec3D.h"

class Triangle {
    
    public:
        Triangle(){};
        Triangle(int v0, int v1, int v2, int n0, int n1, int n2, int t0, int t1, int t2);
        const std::string toString();
        int v0;
        int v1;
        int v2;

        int n0;
        int n1;
        int n2;

        int t0;
        int t1;
        int t2;

        void setEdges(Vec3Df edge0, Vec3Df edge1);
        Vec3Df edge0;
        Vec3Df edge1;
};

#endif 
