
#include "Renderer.h"
#include "../input/Mouse.h"
#include <GL/freeglut.h>

const Vec3Df initialLight = Vec3Df(4,0,-4);
const Vec3Df initialLightColor = Vec3Df(1,1,1);

Renderer::Renderer(){
    meshes.clear();
    camPos = getCameraPosition();
    lightIndex = 0;
    resetLights();
}

void Renderer::replaceLight(Vec3Df lightColor){
    lights[lightIndex] = getCameraPosition();
    lightsColor[lightIndex] = lightColor;
}

void Renderer::addLight(Vec3Df light, Vec3Df lightColor){
    lights.push_back(light);
    lightsColor.push_back(lightColor);
    lightIndex++;
}

void Renderer::addLightAtCamPos(Vec3Df lightColor){
    addLight(getCameraPosition(), lightColor);
}

void Renderer::resetLights(){
    lights.resize(1);
    lights[0] = initialLight;
    lightsColor.resize(1);
    lightsColor[0] = initialLightColor;
    lightIndex = 0;
}

void Renderer::addMesh(Mesh mesh){
    meshes.push_back(mesh);
}

void Renderer::render(){
    //First render the lights
    glPointSize(40);
    glBegin(GL_POINTS);
    glColor3f(1,1,0);
    for (unsigned int i=0; i < lights.size();++i){	
		glVertex3f(lights[i][0],lights[i][1],lights[i][2]);
	}
	glEnd();

    //Now render the meshes
    for(int i = 0; i < meshes.size(); i++){
        meshes[i].drawWithLight();
    }
    for(int i = 0; i < strings.size(); i++){
        renderStrings();
    }
}

void Renderer::idle(){
    camPos=getCameraPosition();
	computeLighting();
	glutPostRedisplay();
}

void Renderer::computeLighting(){
    for(int i = 0; i < meshes.size(); i++){
		meshes[i].computeLighting(lights, camPos);
	}
}

void Renderer::addString(ASCIIString s){
    strings.push_back(s);
}

void Renderer::renderStrings(){  
    for(int i = 0; i < strings.size(); i++){
        const unsigned char* t = reinterpret_cast<const unsigned char *>(strings[i].text);
        glColor3f(strings[i].r, strings[i].g, strings[i].b); 
        glRasterPos2f(strings[i].x, strings[i].y);

        glutBitmapString(GLUT_BITMAP_HELVETICA_18, t);
    }
}