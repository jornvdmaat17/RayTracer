#include <GL/glut.h>
#include "Mesh.h"
#include <cstring>
#include "ObjLoader.h"

Mesh::Mesh(const char* path, Vec3Df pos, int scale) : pos(pos), scale(scale){
    vertices.clear();
    triangles.clear();  
    loadOBJ(path, vertices, triangles);
}

void Mesh::draw(){

    glBegin(GL_TRIANGLES);

    for(int i = 0; i < triangles.size(); i++){

        glVertex3f(vertices[triangles[i].v0].p[0], vertices[triangles[i].v0].p[1] ,vertices[triangles[i].v0].p[2]);
        glVertex3f(vertices[triangles[i].v1].p[0], vertices[triangles[i].v1].p[1] ,vertices[triangles[i].v1].p[2]);
        glVertex3f(vertices[triangles[i].v2].p[0], vertices[triangles[i].v2].p[1] ,vertices[triangles[i].v2].p[2]);
    }


    glEnd();
}


void Mesh::drawWithColors(const std::vector<Vec3Df> & colors){

    glBegin(GL_TRIANGLES);

    for (int i=0;i<triangles.size();++i)    {
        
        const Vec3Df & color0 = colors[triangles[i].v0];
        glColor3f(color0[0], color0[1], color0[2]);
        glNormal3f(vertices[triangles[i].v0].n[0], vertices[triangles[i].v0].n[1], vertices[triangles[i].v0].n[2]);
        glVertex3f(vertices[triangles[i].v0].p[0], vertices[triangles[i].v0].p[1], vertices[triangles[i].v0].p[2]);
        
        const Vec3Df & color1 = colors[triangles[i].v1];
        glColor3f(color1[0], color1[1], color1[2]);
        glNormal3f(vertices[triangles[i].v1].n[0], vertices[triangles[i].v1].n[1], vertices[triangles[i].v1].n[2]);
        glVertex3f(vertices[triangles[i].v1].p[0], vertices[triangles[i].v1].p[1], vertices[triangles[i].v1].p[2]);

        const Vec3Df & color2 = colors[triangles[i].v2];
        glColor3f(color2[0], color2[1], color2[2]);
        glNormal3f(vertices[triangles[i].v2].n[0], vertices[triangles[i].v2].n[1], vertices[triangles[i].v2].n[2]);
        glVertex3f(vertices[triangles[i].v2].p[0], vertices[triangles[i].v2].p[1], vertices[triangles[i].v2].p[2]);

    }
    glEnd();
}

Rectangle::Rectangle(Vec3Df pos, float scale) : Mesh("data/rectangle.obj", pos, scale) {}

Cube::Cube(Vec3Df pos, float scale) : Mesh("data/cube.obj", pos, scale) {}

Sphere::Sphere(Vec3Df pos, float scale) : Mesh("data/sphere.obj", pos, scale){}

void Mesh::printVertices(){
    for(unsigned int i = 0; i < vertices.size(); i++){
        std::cout << vertices[i].p[0] << " " << vertices[i].p[1] << " " << vertices[i].p[2] << std::endl;
    }
}

void Mesh::printTriangles(){
    for(unsigned int i = 0; i < triangles.size(); i++){
        std::cout << triangles[i].v0 << " " << triangles[i].v1 << " " << triangles[i].v2 << std::endl;
    }
}

void Mesh::computeVertexNormals() {
    // Vertex normals initialization
    for (unsigned int i = 0; i < vertices.size (); i++)
        vertices[i].n = Vec3Df (0.0, 0.0, 0.0);
    

    // Sum of  vertex neighbourhood normals
    for (unsigned int i = 0; i < triangles.size(); i++) {
        Vec3Df edge01 = vertices[triangles[i].v1].p -  vertices[triangles[i].v0].p;
        Vec3Df edge02 = vertices[triangles[i].v2].p -  vertices[triangles[i].v0].p;
        Vec3Df n = Vec3Df::crossProduct (edge01, edge02);
        n.normalize ();
        vertices[triangles[i].v0].n += n;
        vertices[triangles[i].v1].n += n;
        vertices[triangles[i].v2].n += n;
    }
    //Normalization
    for (unsigned int i = 0; i < vertices.size (); i++)
        vertices[i].n.normalize();
}