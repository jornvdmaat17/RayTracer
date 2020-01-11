#include "Vertex.h"


/*
Different constructors of the Vertex class
*/
Vertex::Vertex() {}
Vertex::Vertex (const Vec3Df & p) : p (p) {}
Vertex::Vertex (const Vec3Df & p, const Vec3Df & n) : p (p), n (n) {}
Vertex::Vertex (const Vertex & v) : p (v.p), n (v.n) {}
Vertex::~Vertex() {}