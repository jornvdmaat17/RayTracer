#ifndef _obj_loader_h_
#define _obj_loader_h_

#include <vector>
#include "../math/Triangle.h"

class Vertex;

bool loadOBJ(
    const char *path,
    std::vector<Vertex> & out_vertices,
    std::vector<Triangle> & out_triangles,
    std::vector<Vec3Df> & normals
);
    

#endif