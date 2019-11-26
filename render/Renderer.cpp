
#include "Renderer.h"
#include "../input/Mouse.h"
#include <GL/glut.h>

const Vec3Df initialLight = Vec3Df(4,0,-4);
const Vec3Df initialLightColor = Vec3Df(1,1,1);

Renderer::Renderer(){
    camPos = getCameraPosition();
    lightIndex = 0;
    resetLights();
}

void Renderer::addMesh(Mesh mesh){
    meshes.push_back(mesh);
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
