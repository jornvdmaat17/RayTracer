#ifndef _triangle_h_
#define _triangle_h_

#include <string>

class Triangle {
    
    public:
        Triangle();
        Triangle(const Triangle &t);
        Triangle(int x, int y, int z);
        const std::string toString();
        int v0;
        int v1;
        int v2;
        ~Triangle();
};

#endif 
