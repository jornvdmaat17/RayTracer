
#include "DebugDraw.h"
#include <GL/freeglut.h>

DebugDraw::DebugDraw(){}
    
void DebugDraw::addLine(Line l){
    lines.push_back(l);
}

void DebugDraw::addPoint(Point p){
    points.push_back(p);
}

void DebugDraw::clear(){
    lines.clear();
    points.clear();
}

void DebugDraw::drawAll(){
    for(Line l : lines){
        glBegin(GL_LINES);
        glColor3f(l.color.p[0], l.color.p[1], l.color.p[2]);
        glVertex3f(l.p.p[0], l.p.p[1], l.p.p[2]);
        glVertex3f(l.q.p[0], l.q.p[1], l.q.p[2]);
        glEnd();
    }
    for(Point p : points){
        glPointSize(4);
        glBegin(GL_POINTS);
        glColor3f(p.color.p[0], p.color.p[1], p.color.p[2]);
        glVertex3f(p.p.p[0], p.p.p[1], p.p.p[2]);
        glEnd();
    }
}
