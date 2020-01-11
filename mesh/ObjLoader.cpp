#include "ObjLoader.h"
#include <cstring>
#include "../math/Vertex.h"

/*
This function takes in a .obj path and stores its contents in out_vertices, out_triangles, normals, textureCoords

If the file is not availabe it wil return false
The out_triangles will hold all the indices for the vertices, normals and textureCoords

*/
bool loadOBJ(const char *path, std::vector<Vertex> & out_vertices, std::vector<Triangle> & out_triangles, std::vector<Vec3Df> & normals, std::vector<Vec2Df> & textureCoords){

    std::vector<unsigned int> vertexIndices, normalIndices, textureIndices;
    std::vector<Vec3Df> temp_vertices, temp_normals; 
    std::vector<Vec2Df> temp_textures;

    FILE *file = fopen(path, "r");
    if(file == NULL){
        printf("Impossible to open the file \n");
        return false;
    }

    while(1){
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);

        if(res == EOF)
            break;

        if(strcmp(lineHeader, "v") == 0){
            Vec3Df vertex;
            fscanf(file, "%f %f %f\n", &vertex.p[0], &vertex.p[1], &vertex.p[2]);
            temp_vertices.push_back(vertex);
        }else if(strcmp(lineHeader, "vn") == 0){
            Vec3Df normal;
            fscanf(file, "%f %f %f\n", &normal.p[0], &normal.p[1], &normal.p[2]);
            temp_normals.push_back(normal);
            normals.push_back(normal);
        }else if(strcmp(lineHeader, "vt") == 0){
            Vec2Df texture;
            fscanf(file,"%f %f\n", &texture.p[0], &texture.p[1]);
            temp_textures.push_back(texture);
            textureCoords.push_back(texture);
        }else if(strcmp(lineHeader, "f") == 0){
            unsigned int vertexIndex[3], normalIndex[3], uvIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
            &vertexIndex[0], &uvIndex[0], &normalIndex[0],
            &vertexIndex[1], &uvIndex[1], &normalIndex[1],
            &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            if(matches != 9){
                printf("File cant be read by simple parser");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            textureIndices.push_back(uvIndex[0]);
            textureIndices.push_back(uvIndex[1]);
            textureIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }
    
    for(unsigned int i = 0; i < temp_vertices.size(); i++){ 
        Vertex v = Vertex(temp_vertices[i], temp_normals[normalIndices[i] - 1]);
        out_vertices.push_back(v);
    }

    /*Correctly setup the triangles with the right indices*/
    for(unsigned int i = 0; i < vertexIndices.size(); i += 3){
        Triangle t = Triangle(vertexIndices[i] - 1, vertexIndices[i + 1] - 1, vertexIndices[i + 2] - 1, 
                              normalIndices[i] - 1, normalIndices[i + 1] - 1, normalIndices[i + 2] - 1, 
                              textureIndices[i] - 1, textureIndices[i + 1] - 1, textureIndices[i + 2] - 1);
        out_triangles.push_back(t);
    }
    
    return true;
}
