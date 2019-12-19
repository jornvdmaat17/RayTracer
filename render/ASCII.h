#ifndef _ascii_h_
#define _ascii_h_

class ASCIIString {

    public:
        ASCIIString(float x, float y, unsigned int r, unsigned int g, unsigned int b, const char * text);
        float x;
        float y;
        unsigned int r;
        unsigned int g;
        unsigned int b;
        const char * text;    

};

#endif