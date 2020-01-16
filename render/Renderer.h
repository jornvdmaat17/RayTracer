/*
Header file that holds the class Renderer
*/

#ifndef _renderer_h_
#define _renderer_h_

#include <vector>
#include "../mesh/Mesh.h"
#include "ASCII.h"

class Renderer {

    private:
        std::vector<Vec3Df> lightsColor;
        unsigned int lightIndex;

    public:
        std::vector<Mesh> meshes;
        Vec3Df camPos;
        std::vector<Vec3Df> lights;
        std::vector<ASCIIString> strings;
        Renderer();
        void computeLighting();
        void addLight(Vec3Df light, Vec3Df lightColor);
        void addMesh(Mesh mesh);
        void replaceLight(Vec3Df lightColor);
        void addLightAtCamPos(Vec3Df lightColor);
        void resetLights();

        void render();
        void idle();

        void addString(ASCIIString s);
        void renderStrings();
};


#endif