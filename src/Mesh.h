
#pragma once

#include <glad/glad.h>
#include "Material.h"

class Mesh {
public:
    Mesh(GLuint VAO, GLuint VBO, GLsizei count, const Program &program, const Material &material, bool smooth = false);

    void render() const;

    const Program &getProgram() const { return program; }

    void deleteMesh();

    const Material &material;
private:
    const GLuint VAO, VBO;
    const GLsizei count;
    const Program &program;
    const bool smooth;
};