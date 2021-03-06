
#include "Renderer.h"
#include "../input/Mouse.h"
#include <GL/freeglut.h>

// Starting position and color for the light
const Vec3Df initialLight = Vec3Df(4,0,-4);
const Vec3Df initialLightColor = Vec3Df(1,1,1);

/*Constructor that sets the cameraPos and set the lights */
Renderer::Renderer(){
    meshes.clear();
    camPos = getCameraPosition();
    lightIndex = 0;
    resetLights();
}

/*Replaces the current selected light*/
void Renderer::replaceLight(Vec3Df lightColor){
    lights[lightIndex] = getCameraPosition();
    lightsColor[lightIndex] = lightColor;
}

/*Adds a light to the scene*/
void Renderer::addLight(Vec3Df light, Vec3Df lightColor){
    lights.push_back(light);
    lightsColor.push_back(lightColor);
    lightIndex++;
}

/*Adds light at the current camcera position*/
void Renderer::addLightAtCamPos(Vec3Df lightColor){
    addLight(getCameraPosition(), lightColor);
}

/*Reset the lights to the starting position*/
void Renderer::resetLights(){
    lights.resize(1);
    lights[0] = initialLight;
    lightsColor.resize(1);
    lightsColor[0] = initialLightColor;
    lightIndex = 0;
}

/*Add a mesh to the scene*/
void Renderer::addMesh(Mesh mesh){
    meshes.push_back(mesh);
}

/*Render all the lights sources, meshes and strings*/
void Renderer::render(){
    glPointSize(40);
    glBegin(GL_POINTS);
    glColor3f(1,1,0);
    for (unsigned int i=0; i < lights.size();++i){	
		glVertex3f(lights[i][0],lights[i][1],lights[i][2]);
	}
	glEnd();

    for(int i = 0; i < meshes.size(); i++){
        meshes[i].drawWithLight();
    }
    for(int i = 0; i < strings.size(); i++){
        renderStrings();
    }
}

/*Update everything in between draws*/
void Renderer::idle(){
    camPos=getCameraPosition();
	computeLighting();
	glutPostRedisplay();
}

/*Compute the lighting for all the meshes in the scene*/
void Renderer::computeLighting(){
    for(int i = 0; i < meshes.size(); i++){
		meshes[i].computeLighting(lights, camPos);
	}
}

/*Add a string to the scene*/
void Renderer::addString(ASCIIString s){
    strings.push_back(s);
}

/*Render all the strings to the scene*/
void Renderer::renderStrings(){  
    for(int i = 0; i < strings.size(); i++){
        const unsigned char* t = reinterpret_cast<const unsigned char *>(strings[i].text);
        glColor3f(strings[i].r, strings[i].g, strings[i].b); 
        glRasterPos2f(strings[i].x, strings[i].y);

        glutBitmapString(GLUT_BITMAP_HELVETICA_18, t);
    }
}