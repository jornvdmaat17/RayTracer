#ifndef _ray_tracer_h_
#define _ray_tracer_h_

#include "../math/Vec3D.h"
#include <vector>
#include "../mesh/Mesh.h"
#include "Image.h"
#include "DebugDraw.h"

class RayTracer {

    public:
        RayTracer(int screen_width, int screen_height, std::vector<Mesh> meshes, std::vector<Vec3Df> lights, DebugDraw & debugDraw);
        ~RayTracer();
        void startRayTracing();        
        void writeToImage(Image & res);

    private:
        Vec3Df o00, d00, o01, d01, o10, d10, o11, d11;
        std::vector<std::vector<Vec3Df>> rows;
        int width, height;
        std::vector<Mesh> meshes;
        std::vector<Vec3Df> lights;
        DebugDraw &debugDraw;

        void produceRay(int x_I, int y_I, Vec3Df * origin, Vec3Df * dest);
        std::vector<Vec3Df> renderRow(unsigned int y);
        Vec3Df performRayTracing(const Vec3Df & origin, const Vec3Df & dest);
        void rayTraceRecursive(const Vec3Df & o, const Vec3Df & d, Vec3Df & colorOut, Vec3Df & distOut, float & tMin, int depth);
        void calculateLights(Vec3Df & lightDir, Vec3Df & lightPos, const Vec3Df & d, Vec3Df & n, Vec3Df & colorOut, Mesh * mesh);
        void ensurePosDotProd(const Vec3Df & constVec, Vec3Df & varVec);
};


#endif