#include <GL/glut.h>
#include "Mesh.h"
#include <cstring>
#include "ObjLoader.h"
#include <algorithm>

/*
Constructor of a Mesh object
objPath -> where is the .obj file of this mesh stored
mtlPath -> where is the .mtl file of this mesh stored
texturePath -> where is the .bmp file of the mesh stored
pos -> position of Mesh in the scene, default is Vec3Df(0,0,0)
scale -> scale the Mesh
*/
Mesh::Mesh(const char* objPath, const char* mtlPath, const char* texturePath, Vec3Df pos, float scale) : pos(pos), scale(scale){
    vertices.clear();
    triangles.clear();  
    normals.clear();
    textureCoords.clear();
    loadOBJ(objPath, vertices, triangles, normals, textureCoords);
    material = Material(mtlPath);
    texture = Texture(texturePath);
    lighting.resize(vertices.size());
    computeVertexNormals();
    centerAndScaleToUnit();
}

/*
Draw the Mesh with colors which are calculated in calculateLighting
*/
void Mesh::drawWithColors(const std::vector<Vec3Df> & colors){

    glBegin(GL_TRIANGLES);

    for (int i=0;i<triangles.size();++i)    {
        
        const Vec3Df & color0 = colors[triangles[i].v0];
        glColor3f(color0[0], color0[1], color0[2]);
        // glNormal3f(vertices[triangles[i].v0].n[0], vertices[triangles[i].v0].n[1], vertices[triangles[i].v0].n[2]);
        glVertex3f(vertices[triangles[i].v0].p[0], vertices[triangles[i].v0].p[1], vertices[triangles[i].v0].p[2]);
        
        const Vec3Df & color1 = colors[triangles[i].v1];
        glColor3f(color1[0], color1[1], color1[2]);
        // glNormal3f(vertices[triangles[i].v1].n[0], vertices[triangles[i].v1].n[1], vertices[triangles[i].v1].n[2]);
        glVertex3f(vertices[triangles[i].v1].p[0], vertices[triangles[i].v1].p[1], vertices[triangles[i].v1].p[2]);

        const Vec3Df & color2 = colors[triangles[i].v2];
        glColor3f(color2[0], color2[1], color2[2]);
        // glNormal3f(vertices[triangles[i].v2].n[0], vertices[triangles[i].v2].n[1], vertices[triangles[i].v2].n[2]);
        glVertex3f(vertices[triangles[i].v2].p[0], vertices[triangles[i].v2].p[1], vertices[triangles[i].v2].p[2]);

    }
    glEnd();
}

/*Constructor of Cube in which cube.obj is given to the Mesh*/
Cube::Cube(const char *mtlpath, const char* texturePath, Vec3Df pos, float scale) : Mesh("data/cube.obj", mtlpath, texturePath, pos, scale) {}

/*Constructor of Sphere in which sphere.obj is given to the Mesh*/
Sphere::Sphere(const char *mtlpath, const char* texturePath, Vec3Df pos, float scale) : Mesh("data/sphere.obj", mtlpath, texturePath, pos, scale){}

/*Constructor of Plane in which plane.obj is given to the Mesh*/
Plane::Plane(const char * mtlpath, const char * texturePath, Vec3Df pos, float scale) : Mesh("data/plane.obj", mtlpath, texturePath, pos, scale){}

/*Print the Vertices to the console*/
void Mesh::printVertices(){
    for(unsigned int i = 0; i < vertices.size(); i++){
        std::cout << vertices[i].p[0] << " " << vertices[i].p[1] << " " << vertices[i].p[2] << std::endl;
    }
}

/*Print the Triangles to the console*/
void Mesh::printTriangles(){
    for(unsigned int i = 0; i < triangles.size(); i++){
        std::cout << triangles[i].v0 << " " << triangles[i].v1 << " " << triangles[i].v2 << std::endl;
    }
}

/*Print the Edges of the triangles to the console*/
void Mesh::printTriangleEdges(){
    for(unsigned int i = 0; i < triangles.size(); i++){
        std::cout << triangles[i].edge0 << " " << triangles[i].edge1 << std::endl;
    }
}

/*Compute Vertex Normals based on the triangels instead of the vectors itself, these are used in the raytracer*/
void Mesh::computeVertexNormals() {
    // Vertex normals initialization
    for (unsigned int i = 0; i < vertices.size (); i++)
        vertices[i].n = Vec3Df (0.0, 0.0, 0.0);
    
    // Sum of  vertex neighbourhood normals
    for (unsigned int i = 0; i < triangles.size(); i++) {
        Vec3Df edge01 = vertices[triangles[i].v1].p -  vertices[triangles[i].v0].p;
        Vec3Df edge02 = vertices[triangles[i].v2].p -  vertices[triangles[i].v0].p;
        triangles[i].setEdges(edge01, edge02);
        Vec3Df n = Vec3Df::crossProduct(triangles[i].edge0, triangles[i].edge1);
        vertices[triangles[i].v0].n += n;
        vertices[triangles[i].v1].n += n;
        vertices[triangles[i].v2].n += n;
    }
    //Normalization
    for (unsigned int i = 0; i < vertices.size (); i++)
        vertices[i].n.normalize();
}

/*Calculate the lighting of the mesh, based on the lights and camera position*/
void Mesh::computeLighting(std::vector<Vec3Df> & LightPos, Vec3Df & CamPos){
    std::vector<Vec3Df> *result = &lighting;
    for (unsigned int i=0; i < vertices.size();++i){
		(*result)[i] = Vec3Df();
		for (unsigned int l=0; l< LightPos.size();++l){
			(*result)[i] += computeLightingPerVector(vertices[i].p, vertices[i].n, l, LightPos, CamPos);
        }
    }
}

/*Compute the lighting of a vector*/
Vec3Df Mesh::computeLightingPerVector(Vec3Df & vertexPos, Vec3Df & normal, unsigned int light, std::vector<Vec3Df> & LightPos, Vec3Df & CamPos){
	Vec3Df result(0,0,0);
	if (DiffuseLighting){
        result+=diffuseOnly(vertexPos, normal, LightPos[light]);
	}
	if (PhongSpecularLighting){
        result+=phongSpecularOnly(vertexPos, normal, LightPos[light], CamPos);
    }
	else if (BlinnPhongSpecularLighting){
        result+=blinnPhongSpecularOnly(vertexPos, normal, LightPos[light], CamPos);
    }
	return result;
}

/*Draw the mesh with light where we use the calculated lighting*/
void Mesh::drawWithLight(){
    drawWithColors(lighting);
}

/*Calculate the diffuse on a Mesh*/
Vec3Df Mesh::diffuseOnly(const Vec3Df & vertexPos, Vec3Df & normal, const Vec3Df & lightPos){	
	normal.normalize();
	Vec3Df dis = lightPos - vertexPos;
	dis.normalize();
	float t =  std::max(0.f, Vec3Df::dotProduct(normal, dis));
	return (t*material.Kd);
}

/*Calcuate phong specularity on a Mesh*/
Vec3Df Mesh::phongSpecularOnly(const Vec3Df & vertexPos, Vec3Df & normal, const Vec3Df & lightPos, const Vec3Df & cameraPos){
	Vec3Df viewDir = cameraPos - vertexPos;
	viewDir.normalize();
	normal.normalize();
	Vec3Df lightP = lightPos;
	lightP.normalize();
	Vec3Df reflectDir = 2 * Vec3Df::dotProduct(lightP, normal) * normal - lightP;
	float t = std::max((double)0, pow(Vec3Df::dotProduct(viewDir, reflectDir), material.Sh));
	return (t * material.Ks);
}

/*Calculate blinn-phong specularity on a Mesh*/
Vec3Df Mesh::blinnPhongSpecularOnly(const Vec3Df & vertexPos, Vec3Df & normal, const Vec3Df & lightPos, const Vec3Df & cameraPos){
	normal.normalize();
	Vec3Df lightDir = lightPos - vertexPos;
	lightDir.normalize();
	Vec3Df viewDir = cameraPos - vertexPos;
	viewDir.normalize();
	Vec3Df halfDir = lightDir + viewDir;
	halfDir.normalize();
	float t = std::max((double)0, pow(Vec3Df::dotProduct(normal, halfDir), material.Sh));
	return ( t * material.Ks);
}

/*Center and translate the Mesh to the correct position and scale*/
void Mesh::centerAndScaleToUnit() {
    Vec3Df c;
    for  (unsigned int i = 0; i < vertices.size (); i++)
        c += vertices[i].p;
    c /= vertices.size();
    float maxD = Vec3Df::distance(vertices[0].p, c);
    for (unsigned int i = 0; i < vertices.size(); i++){
        float m = Vec3Df::distance (vertices[i].p, c);
        if (m > maxD)
            maxD = m;
    }
    for  (unsigned int i = 0; i < vertices.size (); i++)
        vertices[i].p = (vertices[i].p - c + pos) * scale / maxD;
}	