/*

Header file for a Vertex class with vector coordinates and normal directions

*/

#ifndef _vertex_h
#define _vertex_h_

#include "Vec3D.h"

class Vertex {

    public:
        Vertex ();
        Vertex (const Vec3Df & p);
        Vertex (const Vec3Df & p, const Vec3Df & n);
        Vertex (const Vertex & v);
        ~Vertex ();
        
        Vec3Df p;
        Vec3Df n;
};


#endif 
