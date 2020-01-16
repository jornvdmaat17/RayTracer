#include "RayTracer.h"
#include <GL/glut.h>
#include <iostream>
#include "../input/Mouse.h"
#include <algorithm>
#include <fstream>

// Variables
const float BackgroundColor[]={0.2,0.3,0};
float epsilon = 0.000000000000001f;

// Map function
float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*Constructors*/
RayTracer::RayTracer(int screen_width, int screen_height, std::vector<Mesh> meshes, std::vector<Vec3Df> lights, DebugDraw & debugDraw) : width(screen_width), height(screen_height), debugDraw(debugDraw) {
    this->meshes = meshes;
    this->lights = lights;
    produceRay(0, 0, &o00, &d00);
    produceRay(0, screen_height - 1, &o01, &d01);
    produceRay(screen_width - 1, 0, &o10, &d10);
    produceRay(screen_width - 1, screen_height - 1, &o11, &d11);
}

RayTracer::~RayTracer(){}

/*Produces the rays for the whole screen, from corner to corner*/
void RayTracer::produceRay(int x_I, int y_I, Vec3Df * origin, Vec3Df * dest){
    int viewport[4];
    double modelview[16];
    double projection[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview); 
    glGetDoublev(GL_PROJECTION_MATRIX, projection); 
    glGetIntegerv(GL_VIEWPORT, viewport);
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

/*
Renders a single row of the program by rendering all the different pixels on  a row
Pushes the result of performRaytracing to results
*/
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

/*Starts the RayTracer by starting initializing all the rows and pushing back the results*/
void RayTracer::startRayTracing(){
    std::cout << "Started RayTracing" << std::endl;
    std::vector<std::future<std::vector<Vec3Df>>> li;
    for(unsigned int i = 0; i < height; i++){
        li.push_back(std::async(std::launch::async, &RayTracer::renderRow, this, i));
    }
    
    for(unsigned int i = 0; i < height; i++){
        rows.push_back(li[i].get());
        std::cout << 100.f * i / HEIGHT << "%\r\n";
    }
    std::cout << "Done RayTracking" << std::endl;
}

/*Function that calls the rayTraceRecursive function with the correct parameters*/
Vec3Df RayTracer::performRayTracing(const Vec3Df & origin, const Vec3Df & dest){
	Vec3Df color(0, 0, 0);
	Vec3Df dist(0, 0, 0);
    float t;
	Vec3Df d = dest - origin;
	d.normalize();
    rayTraceRecursive(origin, d, color, dist, t, 2);
	return color;
}

/*
Recursive ray trace function, does several things:

First it calculates the first triangle the ray intersects with
Then it calculates the diffusion and shading at that triangle and calculates the possible reflection in an object
It returns the color in colorOut and uses some other variables too, which are returned by reference
*/
void RayTracer::rayTraceRecursive(const Vec3Df & o, const Vec3Df & d, Vec3Df & colorOut, Vec3Df & distOut, float & tMin, int depth){
    float nearestT = MAXFLOAT;
    unsigned int tIndex;
    unsigned int mIndex;
    float aMin, bMin;
    Vec3Df pMin(0,0,0);     

    // Walk through all the meshes
    for(unsigned int meshIndex = 0; meshIndex < meshes.size(); meshIndex++){
        std::vector<Vertex> & vertices = meshes[meshIndex].vertices;
        std::vector<Triangle> & triangles = meshes[meshIndex].triangles;    

        // Walk through all the triangles of the selected mesh
        for(unsigned int triangleIndex = 0; triangleIndex < triangles.size(); triangleIndex++){
            Vec3Df v0 = vertices[triangles[triangleIndex].v0].p;
            Vec3Df v1 = vertices[triangles[triangleIndex].v1].p;
            Vec3Df v2 = vertices[triangles[triangleIndex].v2].p;
           
            // Start the intersection algorithm           
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

            // The distance between the origin and hitpoint on the ray
            float t = f * Vec3Df::dotProduct(edge2, q);

            // If this new t is the closest triangle, calculate important variables
            if (t > epsilon && t < nearestT) {
                Vec3Df p = o + t * d;                
            
                Vec3Df v0p = p - v0;
                Vec3Df v1p = p - v1;
                Vec3Df v2p = p - v2;
                Vec3Df N = Vec3Df::crossProduct((v1 - v0), (v2 - v0));
                float denom = Vec3Df::dotProduct(N, N);

                nearestT = t;
                tIndex = triangleIndex;
                mIndex = meshIndex;
                pMin = p;
                aMin = Vec3Df::dotProduct(N, Vec3Df::crossProduct((v2 - v1), v1p)) / denom;
                bMin = Vec3Df::dotProduct(N, Vec3Df::crossProduct((v0 - v2), v2p)) / denom;
                
            }
        }
    }

    // After all the triangles, go on with calculating shading, diffusion and reflection
    tMin = nearestT;
    Vec3Df tempCollerAddition(0,0,0);
    Vec3Df tempColorOut(0,0,0);
    Vec3Df ownColor;
    if(nearestT < MAXFLOAT){
        distOut = pMin;

        if(depth > 0){
            Mesh * mesh = &meshes[mIndex];
            Texture t = mesh->texture;

            Triangle hitTriangle = mesh->triangles[tIndex];
            Vec3Df v0n = mesh->vertices[hitTriangle.v0].n;
            Vec3Df v1n = mesh->vertices[hitTriangle.v1].n;
            Vec3Df v2n = mesh->vertices[hitTriangle.v2].n;

            pMin.normalize();

            Vec3Df n = aMin * v0n + bMin * v1n + (1.f - aMin - bMin ) * v2n;
            n.normalize();

            // Calculate shading and diffusion for eacht lightsource
            for(Vec3Df lightPos : lights){
                Vec3Df lightDir = lightPos - pMin;   
                
                Vec2Df t0 = mesh->textureCoords[hitTriangle.t0];
                Vec2Df t1 = mesh->textureCoords[hitTriangle.t1];
                Vec2Df t2 = mesh->textureCoords[hitTriangle.t2];

                Vec2Df pixel = aMin * t0 + bMin * t1 + (1.f - aMin - bMin) * t2;
                Vec3Df colour = t.getPixelColor(pixel.p[0] * t.width, pixel.p[1] * t.height);
                
                tempColorOut = Vec3Df(map(colour.p[0], 0.f, 255.f, 0.f, 1.f), map(colour.p[1], 0.f, 255.f, 0.f, 1.f), map(colour.p[2], 0.f, 255.f, 0.f, 1.f));
                calculateLights(lightDir, lightPos, d, n, tempColorOut, mesh);
                tempCollerAddition += tempColorOut;
            }
            ownColor = tempCollerAddition / lights.size();    

            // Set reflSc to 0 if the depth is itself
            float reflSc;
            if(depth == 1)
                reflSc = 0.f;
            else
                reflSc = mesh->material.Rs;

            Vec3Df reflectedColor;

            // Calculate reflection of ray, by using the rayTraceRecursive function, with different parameters
            if(reflSc > 0){
                ensurePosDotProd(d, n);

                float raySurfaceDotProduct = Vec3Df::dotProduct(d, n);
                Vec3Df reflection = d - 2.f * n * raySurfaceDotProduct;
                reflection.normalize();

                Vec3Df reflectionPoint = distOut + reflection * 0.55f;

                float ignoreDist;
                Vec3Df ignorePos;

                rayTraceRecursive(reflectionPoint, reflection, reflectedColor, ignorePos, ignoreDist, depth - 1);

                if(reflectedColor == Vec3Df(BackgroundColor[0], BackgroundColor[1], BackgroundColor[2])){
                    reflectedColor = ownColor;
                }
            }

            // Calculate the final color
            colorOut = (1 - reflSc) * ownColor + reflSc * reflectedColor;
        }
    }else{
        // If there is no intersection set the color to the backgroundColor
        colorOut = Vec3Df(BackgroundColor[0], BackgroundColor[1], BackgroundColor[2]);
    }
}

/*Function for calculating shading and diffusion*/
void RayTracer::calculateLights(Vec3Df & lightDir, Vec3Df & lightPos, const Vec3Df & d, Vec3Df & n, Vec3Df & colorOut, Mesh * mesh){
    Vec3Df lightDirUnit = lightDir;
	lightDirUnit.normalize();

	float expectedDist = lightDir.getLength();
    
    Vec3Df ignored;
    float distOut;
	rayTraceRecursive(lightPos, lightDirUnit, ignored, ignored, distOut, 0);

    if (distOut >= expectedDist - epsilon) {
        // Diffuse
        float intensity = Vec3Df::dotProduct(n, lightDirUnit);
        if(intensity < 0) intensity = 0;

        colorOut += intensity * mesh->material.Kd;

        // Shading
        Vec3Df reflection = 2.f * n * intensity - lightDirUnit;
        float dotProd = pow(Vec3Df::dotProduct(reflection, d), mesh->material.Sh);
        if (dotProd > 0) {
            colorOut += dotProd * mesh->material.Ks;
        }
    }
}

/*Write all the rows to image*/
void RayTracer::writeToImage(Image & result){
    for(unsigned int y = 0; y < height; y++){
        for(unsigned int x = 0; x < width; x++){
            Vec3Df row = rows[y][x];
            RGB rgb = RGB(row.p[0], row.p[1], row.p[2]);
            result.setPixel(x, y, rgb);
        }
    }
}

/*Helper function for checking if normals are not inversed*/
void RayTracer::ensurePosDotProd(const Vec3Df & constVec, Vec3Df & varVec) {
	if (Vec3Df::dotProduct(constVec, varVec) < 0) {
		varVec = -varVec;
	}
}
