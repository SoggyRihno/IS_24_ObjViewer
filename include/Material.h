//
// Created by Ian on 5/12/2024.
//

#ifndef OPENGLTEST_MATERIAL_H
#define OPENGLTEST_MATERIAL_H

#include <glm/glm.hpp>
#include "Program.h"
#include "Texture.h"

class Material {

public:
    std::string name;
    glm::vec3 Ka, Kd, Ks, Ke;
    float Ns, Ni, Tr, Tf;
    int illum;
    Texture* map_Ks, map_Ka, map_Kd, map_bumb;

    Material();
};


#endif