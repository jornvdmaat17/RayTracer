#include "SceneBuilder.h"
#include <fstream>
#include <iostream>
#include <string>
#include "../math/Vec3D.h"

std::vector<Mesh> getMeshInformation(const char* f){
    std::string line;
    std::ifstream file(f);
    std::vector<Mesh> meshes;

    if(file){
        while(getline(file, line)){
            if(line.at(0) != '/'){
                std::vector<std::string> res = split(line, " ");
                if(res.size() != 5){
                    std::cout << "File format incorrect" << std::endl;
                    break;
                }
                if(res[0] == "cube"){
                    meshes.push_back(Cube(Vec3Df(std::stof(res[1]), std::stof(res[2]), std::stof(res[3])), std::stof(res[4])));
                    continue;
                }
                if(res[0] == "sphere"){
                    meshes.push_back(Sphere(Vec3Df(std::stof(res[1]), std::stof(res[2]), std::stof(res[3])), std::stof(res[4])));
                    continue;
                }
            }
        }
        file.close();
    }else {
        std::cout << "File " << f << " could not be found" << std::endl;
    }
    return meshes;
}

std::vector<std::string> split(const std::string& str, const std::string& delim){
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}