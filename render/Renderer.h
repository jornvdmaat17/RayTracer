#ifndef _renderer_h_
#define _renderer_h_

#include <vector>
#include "../mesh/Mesh.h"

class Renderer {

    private:
        std::vector<Mesh> meshes;
        std::vector<Vec3Df> lights;
        std::vector<Vec3Df> lightsColor;
        Vec3Df camPos;
        unsigned int lightIndex;
        

    public:
        void computeLighting();
        Renderer();
        void addMesh(Mesh mesh);
        void addLight(Vec3Df light, Vec3Df lightColor);
        void replaceLight(Vec3Df lightColor);
        void addLightAtCamPos(Vec3Df lightColor);
        void resetLights();

        void render();
        void idle();
};


#endif