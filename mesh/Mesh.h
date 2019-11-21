#ifndef _mesh_h_
#define _mesh_h_

#include "../math/Triangle.h"
#include "../math/Vertex.h"
#include <vector>

class Mesh {

    public:
        Mesh(const char* path, Vec3Df pos, int scale);
        
        int vertexCount;
        std::vector<Vertex> vertices;
        std::vector<Triangle> triangles;
        Vec3Df pos;
        float scale;

        void computeVertexNormals();

        void drawWithColors(const std::vector<Vec3Df> & colors);
        void draw();

        void printVertices();
        void printTriangles();  
};

class Cube : public Mesh {
    public:
        Cube(Vec3Df pos, float scale);
};

class Rectangle : public Mesh {
    public:
        Rectangle(Vec3Df pos, float scale);
};

class Sphere : public Mesh {
    public:
        Sphere(Vec3Df pos, float scale);
};


// Add more objects



#endif