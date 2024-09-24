
#pragma once


#include "glad/glad.h"
#include "Material.h"
#include "Loader.h"

class Mesh {
public:
    Mesh(ProgramType programType, const std::vector<float> &vertices, Material material = {});

    ~Mesh();

    void render() const;

private:
    GLuint VAO, VBO;
    size_t count;
    Material material;
    ProgramType programType;
};
