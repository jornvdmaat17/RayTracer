#include <GL/glut.h>
#include "Mesh.h"
#include <cstring>
#include "ObjLoader.h"

Mesh::Mesh(const char* path, Vec3Df pos, int scale) : pos(pos), scale(scale){
    vertices.clear();
    triangles.clear();  
    loadOBJ(path, vertices, triangles);
    Kd.resize(vertices.size(), Vec3Df(0.5,0.5,0.5));
	Ks.resize(vertices.size(), Vec3Df(0.2,0.2,0.2));
	Shininess.resize(vertices.size(), 3);
    lighting.resize(vertices.size());
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

void Mesh::computeLighting(std::vector<Vec3Df> LightPos, Vec3Df CamPos){
    std::vector<Vec3Df> *result = &lighting;
    for (unsigned int i=0; i < vertices.size();++i)
	{
		(*result)[i] = Vec3Df();
		for (unsigned int l=0; l< LightPos.size();++l){
			(*result)[i] += computeLightingPerVector(vertices[i].p, vertices[i].n, l, i, LightPos, CamPos);
        }
    }
}

Vec3Df Mesh::computeLightingPerVector(Vec3Df & vertexPos, Vec3Df & normal, unsigned int light, unsigned int index, std::vector<Vec3Df> LightPos, Vec3Df CamPos){
	Vec3Df result(0,0,0);
	if (DiffuseLighting){
        result+=diffuseOnly(vertexPos, normal, LightPos[light], index);
	}
	if (PhongSpecularLighting){
        result+=phongSpecularOnly(vertexPos, normal, LightPos[light], CamPos, index);
    }
	else if (BlinnPhongSpecularLighting){
        result+=blinnPhongSpecularOnly(vertexPos, normal, LightPos[light], CamPos, index);
    }
	return result;
}

void Mesh::drawWithLight(){
    drawWithColors(lighting);
}

Vec3Df Mesh::diffuseOnly(const Vec3Df & vertexPos, Vec3Df & normal, const Vec3Df & lightPos, unsigned int index)
{	
	normal.normalize();
	Vec3Df dis = lightPos - vertexPos;
	dis.normalize();
	float t =  Vec3Df::dotProduct(normal, dis);
    std::cout << t << std::endl;
	if (t < 0) {
		t = 0;
	}
	Vec3Df res = (t*Kd[index]);
	return res;
}

Vec3Df Mesh::phongSpecularOnly(const Vec3Df & vertexPos, Vec3Df & normal, const Vec3Df & lightPos, const Vec3Df & cameraPos, unsigned int index)
{
	Vec3Df viewDir = cameraPos- vertexPos;
	viewDir.normalize();
	normal.normalize();
	Vec3Df lightP = lightPos;
	lightP.normalize();
	Vec3Df reflectDir = 2 * Vec3Df::dotProduct(lightP, normal) * normal - lightP;

	float t = pow(Vec3Df::dotProduct(viewDir, reflectDir), Shininess[index]);
	if (t < 0) t = 0;
	Vec3Df res =(t * Ks[index]);
	return res;
}

Vec3Df Mesh::blinnPhongSpecularOnly(const Vec3Df & vertexPos, Vec3Df & normal, const Vec3Df & lightPos, const Vec3Df & cameraPos, unsigned int index)
{
	normal.normalize();
	Vec3Df lightDir = lightPos - vertexPos;
	lightDir.normalize();
	Vec3Df viewDir = cameraPos - vertexPos;
	viewDir.normalize();
	Vec3Df halfDir = lightDir + viewDir;
	halfDir.normalize();

	float t = pow(Vec3Df::dotProduct(normal, halfDir), Shininess[index]);
	if (t < 0) t = 0;
	Vec3Df res = ( t * Ks[index]);
	return res;
}
