#include "SceneBuilder.h"
#include <fstream>
#include <iostream>
#include <string>
#include "../math/Vec3D.h"

/*
Function that takes all the information from a file and processes this to the scene.
The format can be found in scene.txt
The function errors when the file cannot be found
*/
std::vector<Mesh> getMeshInformation(const char* f){
    std::string line;
    std::ifstream file(f);
    std::vector<Mesh> meshes;

    if(file){
        while(getline(file, line)){
            if(line.at(0) != '/'){
                std::vector<std::string> res = split(line, " ");
                if(res.size() != 7){
                    std::cout << "File format incorrect" << std::endl;
                    break;
                }
                if(res[0] == "cube"){
                    meshes.push_back(Cube(("data/" + res[5] + ".mtl").c_str(), ("data/" + res[6]).c_str(), Vec3Df(std::stof(res[1]), std::stof(res[2]), std::stof(res[3])), std::stof(res[4])));
                    continue;
                }
                if(res[0] == "sphere"){
                    meshes.push_back(Sphere(("data/" + res[5] + ".mtl").c_str(), ("data/" + res[6]).c_str(), Vec3Df(std::stof(res[1]), std::stof(res[2]), std::stof(res[3])), std::stof(res[4])));
                    continue;
                }
                if(res[0] == "plane"){
                    meshes.push_back(Plane(("data/" + res[5] + ".mtl").c_str(), ("data/" + res[6]).c_str(), Vec3Df(std::stof(res[1]), std::stof(res[2]), std::stof(res[3])), std::stof(res[4])));
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

/*
Split function that splits a string by a delimiter.
Returns a vector with all the splitted strings.
*/
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