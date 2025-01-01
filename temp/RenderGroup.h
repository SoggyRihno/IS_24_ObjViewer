#pragma once

#include <vector>
#include "glad/glad.h"
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