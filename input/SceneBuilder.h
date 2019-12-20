#ifndef _scene_builder_h_
#define _scene_builder_h_

#include <vector>
#include "../mesh/Mesh.h"

std::vector<Mesh> getMeshInformation(const char* f);
std::vector<std::string> split(const std::string& str, const std::string& delim);

#endif
