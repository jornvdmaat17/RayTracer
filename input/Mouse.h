/*
Header file with functions for mouse movement in the scene.
Implementation and details can be found in Mouse.cpp
*/
#ifndef _mouse_h_
#define _mouse_h_

#include <GL/glut.h>
#include "../math/Vec3D.h"

void tbInitTransform();
void tbVisuTransform();
void tbHelp();
void tbMouseFunc(int button, int state, int x, int y);
void tbMotionFunc(int x, int y);
void tbRotate(double angle, double x, double y, double z);
void tbProject( const GLdouble *m, const GLdouble* p, GLdouble* q);
void tbProject( const GLdouble* p, GLfloat* q);
void changeZoom(float amount);
Vec3Df getCameraPosition();
Vec3Df getWorldPositionOfPixel(unsigned int px, unsigned int py);

#endif