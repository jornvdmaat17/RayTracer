#ifndef _matrix_h_
#define _matrix_h_

#include <GL/glut.h>

void project(const GLdouble* m, const GLdouble* p, GLdouble* q);
void project(const GLdouble* m, const GLfloat* p, GLfloat* q);
void project(const GLdouble* m, const GLdouble* p, GLfloat* q);
void inverse(const double *m, double *p);

#endif