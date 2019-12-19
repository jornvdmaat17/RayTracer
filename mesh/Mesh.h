#ifndef _mesh_h_
#define _mesh_h_

#include "../math/Triangle.h"
#include "../math/Vertex.h"
#include <vector>

class Mesh {

    public:
        Mesh(const char* path, Vec3Df pos, int scale);
        Vec3Df pos;
        float scale;

        std::vector<Vec3Df> Kd;
        std::vector<Vec3Df> Ks;
        std::vector<float> Shininess;
        std::vector<Vec3Df> lighting;

        std::vector<Vertex> vertices;
        std::vector<Triangle> triangles;

        bool DiffuseLighting = true;
        bool PhongSpecularLighting = false;
        bool BlinnPhongSpecularLighting = true;

        void drawWithLight();
        void drawWithColors(const std::vector<Vec3Df> & colors);
        void draw();

        void printVertices();
        void printTriangles(); 
        void printTriangleEdges(); 

        void computeLighting(std::vector<Vec3Df> & LightPos, Vec3Df & CamPos); 

    private:
        void computeVertexNormals();
        void centerAndScaleToUnit();
        Vec3Df computeLightingPerVector(Vec3Df & vertexPos, Vec3Df & normal, unsigned int light, unsigned int index, std::vector<Vec3Df> & LightPos, Vec3Df & CamPos);
        Vec3Df diffuseOnly(const Vec3Df & vertexPos, Vec3Df & normal, const Vec3Df & lightPos, unsigned int index);
        Vec3Df phongSpecularOnly(const Vec3Df & vertexPos, Vec3Df & normal, const Vec3Df & lightPos, const Vec3Df & cameraPos, unsigned int index);
        Vec3Df blinnPhongSpecularOnly(const Vec3Df & vertexPos, Vec3Df & normal, const Vec3Df & lightPos, const Vec3Df & cameraPos, unsigned int index);

};

class Cube : public Mesh {
    public:
        Cube(Vec3Df pos, float scale);
};

class Sphere : public Mesh {
    public:
        Sphere(Vec3Df pos, float scale);
};


// Add more objects



#endif