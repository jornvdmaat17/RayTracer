#include "RayTracer.h"
#include <GL/glut.h>
#include <iostream>
#include "../input/Mouse.h"

float epsilon = 0.000000000000001f;
const float BackgroundColor[]={0.2,0.2,0};

RayTracer::RayTracer(int screen_width, int screen_height, std::vector<Mesh> & meshes) : width(screen_width), height(screen_height) {
    this->meshes = meshes;
    produceRay(0, 0, &o00, &d00);
    produceRay(0, screen_height - 1, &o01, &d01);
    produceRay(screen_width - 1, 0, &o10, &d10);
    produceRay(screen_width - 1, screen_height - 1, &o11, &d11);
}

RayTracer::~RayTracer(){}

void RayTracer::produceRay(int x_I, int y_I, Vec3Df * origin, Vec3Df * dest){
    int viewport[4];
    double modelview[16];
    double projection[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview); // get current matrices
    glGetDoublev(GL_PROJECTION_MATRIX, projection); // get current matrices
    glGetIntegerv(GL_VIEWPORT, viewport);//viewport
    int y_new = viewport[3] - y_I;

    double x, y, z;
    
    gluUnProject(x_I, y_new, 0, modelview, projection, viewport, &x, &y, &z);
    origin->p[0]=float(x);
    origin->p[1]=float(y);
    origin->p[2]=float(z);
    gluUnProject(x_I, y_new, 1, modelview, projection, viewport, &x, &y, &z);
    dest->p[0]=float(x);
    dest->p[1]=float(y);
    dest->p[2]=float(z);
}

std::vector<Vec3Df> RayTracer::renderRow(unsigned int y) {
    std::vector<Vec3Df> results;
    Vec3Df origin, dest;
    for (unsigned int x = 0; x < width; ++x) {
        //produce the rays for each pixel, by interpolating
        //the four rays of the frustum corners.
        float xscale = 1.0f - float(x) / width;
        float yscale = 1.0f - float(y) / height;
        
        origin = yscale*(xscale* o00 + (1 - xscale)* o10) +
        (1 - yscale)*(xscale* o01 + (1 - xscale)* o11);
        dest = yscale*(xscale* d00 + (1 - xscale)* d10) +
        (1 - yscale)*(xscale* d01 + (1 - xscale)* d11);
        
        results.push_back(performRayTracing(origin, dest));
    }
    return results;
}


void RayTracer::startRayTracing(){
    std::cout << "Started RayTracing" << std::endl;
    for(unsigned int i = 0; i < height; i++){
        rows.push_back(renderRow(i));
    }
    std::cout << "Done RayTracking" << std::endl;
}

Vec3Df RayTracer::performRayTracing(const Vec3Df & origin, const Vec3Df & dest){
	// float dist;
	Vec3Df color(0, 0, 0);
	Vec3Df dist(0, 0, 0);
	Vec3Df d = dest - origin;
	d.normalize();
	// rayTraceRecursive(origin, d, 2, p, color, dist);
    rayTraceRecursive(origin, d, color, dist);
	return color;
}

void RayTracer::rayTraceRecursive(const Vec3Df & o, const Vec3Df & d, Vec3Df & colorOut, Vec3Df & distOut){ //, float & distOut, int depth, Vec3Df & pOut,){
    float nearestT = MAXFLOAT;
    unsigned int tIndex;
    unsigned int mIndex;
   
    for(unsigned int meshIndex = 0; meshIndex < meshes.size(); meshIndex++){
        std::vector<Vertex> & vertices = meshes[meshIndex].vertices;
        std::vector<Triangle> & triangles = meshes[meshIndex].triangles;

        for(unsigned int triangleIndex = 0; triangleIndex < triangles.size(); triangleIndex++){
            Vec3Df v0 = vertices[triangles[triangleIndex].v0].p;
            Vec3Df v1 = vertices[triangles[triangleIndex].v1].p;
            Vec3Df v2 = vertices[triangles[triangleIndex].v2].p;
            Vec3Df edge1, edge2, h, s, q;
            float a, f, u, v;
            edge1 = v1 - v0;
            edge2 = v2 - v0;
            h = Vec3Df::crossProduct(d, edge2);
            a = Vec3Df::dotProduct(edge1, h);
            if(a > -epsilon && a < epsilon){
                continue;
            }
            
            f = 1.0 / a;
            s = o - v0;
            u = f * Vec3Df::dotProduct(s, h);

            if (u < 0.0 || u > 1.0){
                continue;
            }
            
            q = Vec3Df::crossProduct(s, edge1);
            v = f * Vec3Df::dotProduct(d, q);
            if (v < 0.0 || u + v > 1.0){
                continue;
            }

            float t = f * Vec3Df::dotProduct(edge2, q);
            if (t > epsilon && t < 1 / epsilon) {
                if(t < nearestT){
                    nearestT = t;
                    tIndex = triangleIndex;
                    mIndex = meshIndex;
                }
            }
        }
    }
    if(nearestT < MAXFLOAT){
        Mesh * mesh = &meshes[mIndex];
        colorOut = mesh->lighting[mesh->triangles[tIndex].v0] + mesh->lighting[mesh->triangles[tIndex].v1] + mesh->lighting[mesh->triangles[tIndex].v2] / 3;
    }else{
        colorOut = Vec3Df(BackgroundColor[0], BackgroundColor[1], BackgroundColor[2]);
    }
}

void RayTracer::writeToImage(Image & result){
    for(unsigned int y = 0; y < height; y++){

        for(unsigned int x = 0; x < height; x++){
            Vec3Df row = rows[x][y];
            RGB rgb = RGB(row[0], row[1], row[2]);
            result.setPixel(x, y, rgb);
        }
    }
}

