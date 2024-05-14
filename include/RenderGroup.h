//
// Created by Ian on 5/12/2024.
//

#ifndef OPENGLTEST_RENDERGROUP_H
#define OPENGLTEST_RENDERGROUP_H


#include <vector>
#include <glad/glad.h>
#include "glm/vec3.hpp"
#include "Face.h"
#include "Material.h"
#include "Program.h"

class RenderGroup {

private:
    unsigned int VAO, VBO;
    Material* material;
    glm::vec3 color; // random
    int count, mode;
    bool normal, texture;

public:
    RenderGroup(std::vector<Face> &faces, Material *material);
    void render(Program &program) const;
};


#endif //OPENGLTEST_RENDERGROUP_H
