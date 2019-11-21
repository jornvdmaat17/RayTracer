
#include "Triangle.h"

Triangle::Triangle () {
    v0 = v1 = v2 = 0;
}

Triangle::Triangle (const Triangle &t) {
    v0 = t.v0;
    v1 = t.v1;
    v2 = t.v2;
}

Triangle::Triangle (int v0, int v1, int v2) {
    this->v0 = v0;
    this->v1 = v1;
    this->v2 = v2;
}

const std::string Triangle::toString(){
    return "Triangle(" + std::to_string(v0) + std::string(", ") + std::to_string(v1) + std::string(", ") + std::to_string(v2) + std::string(")");
}

Triangle::~Triangle () {}