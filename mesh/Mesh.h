/*

Header file that holds the Mesh class and its members: Cube, Sphere and Plane

Mesh consists of vertices, normals, texturecoords and triangles with indices to combine those
The function in Mesh are used for the customizable Scene which is started when the program launches
Different lighting and shading can be chosen.
Functions implemtation can be found in Mesh.cpp

*/

#ifndef _mesh_h_
#define _mesh_h_

#include "../math/Triangle.h"
#include "../math/Vertex.h"
#include "Material.h"
#include "Texture.h"
#include <vector>
#include "../math/Vec2D.h"

class Mesh {

    public:
        Mesh(const char* objPath, const char* mtlPath, const char* texturePath, Vec3Df pos, float scale);
        Vec3Df pos;
        float scale;

        Material material;
        Texture texture;
        std::vector<Vec3Df> lighting;

        std::vector<Vertex> vertices;
        std::vector<Triangle> triangles;
        std::vector<Vec3Df> normals;
        std::vector<Vec2Df> textureCoords;

        bool DiffuseLighting = true;
        bool PhongSpecularLighting = true;
        bool BlinnPhongSpecularLighting = false;

        void drawWithLight();
        void drawWithColors(const std::vector<Vec3Df> & colors);
        void draw();

        void printVertices();
        void printTriangles(); 
        void printTriangleEdges(); 

        Vec3Df & normalAt(const int t, const int i);

        void computeLighting(std::vector<Vec3Df> & LightPos, Vec3Df & CamPos); 

    private:
        void computeVertexNormals();
        void centerAndScaleToUnit();
        Vec3Df computeLightingPerVector(Vec3Df & vertexPos, Vec3Df & normal, unsigned int light, std::vector<Vec3Df> & LightPos, Vec3Df & CamPos);
        Vec3Df diffuseOnly(const Vec3Df & vertexPos, Vec3Df & normal, const Vec3Df & lightPos);
        Vec3Df phongSpecularOnly(const Vec3Df & vertexPos, Vec3Df & normal, const Vec3Df & lightPos, const Vec3Df & cameraPos);
        Vec3Df blinnPhongSpecularOnly(const Vec3Df & vertexPos, Vec3Df & normal, const Vec3Df & lightPos, const Vec3Df & cameraPos);

};

class Cube : public Mesh {
    public:
        Cube(const char * mtlpath, const char* texturePath, Vec3Df pos, float scale);
};

class Sphere : public Mesh {
    public:
        Sphere(const char * mtlpath, const char* texturePath, Vec3Df pos, float scale);
};

class Plane : public Mesh {
    public:
        Plane(const char * mtlPath, const char* texturePath, Vec3Df pos, float scale);
};



#endif