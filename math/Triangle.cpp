
#include "Triangle.h"

Triangle::Triangle (int v0, int v1, int v2, int n0, int n1, int n2, int t0, int t1, int t2) : v0(v0), v1(v1), v2(v2), n0(n0), n1(n1), n2(n2), t0(t0), t1(t1), t2(t2) {}

void Triangle::setEdges(Vec3Df edge0, Vec3Df edge1){
    this->edge0 = edge0;
    this->edge1 = edge1;
}

const std::string Triangle::toString(){
    return "Triangle(" + std::to_string(v0) + std::string(", ") + std::to_string(v1) + std::string(", ") + std::to_string(v2) + std::string(")");
}

Triangle::~Triangle () {}
