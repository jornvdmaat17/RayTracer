/*
Header file for the DebugDraw class which is used to debug the program, here you can draw intersection points of draw lines from normals to intersection points.
Not used in the main program.

Holds the classes Point and Line too
*/

#ifndef _debug_draw_h_
#define _debug_draw_h_

#include "../math/Vec3D.h"
#include <vector>

class Point {

    public:
        inline Point(Vec3Df p, Vec3Df color) : p(p), color(color){};
        Vec3Df p;
        Vec3Df color; 
};

class Line {

    public:
        inline Line(Vec3Df p, Vec3Df q, Vec3Df color) : p(p), q(q), color(color){};
        Vec3Df p;
        Vec3Df q;
        Vec3Df color;
};

class DebugDraw {

    private:
        std::vector<Line> lines;
        std::vector<Point> points;

    public:
        DebugDraw();
        void addLine(Line l);
        void addPoint(Point p);
        void drawAll();
        void clear();
};


#endif