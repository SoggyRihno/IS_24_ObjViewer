
#include "Mesh.h"

Mesh::Mesh(GLuint VAO, GLuint VBO, GLsizei count, const Program& program, const Material & material)
    : VAO(VAO), VBO(VBO), count(count), program(program), material(material) {}

//do not add to destructor causes to many problems with copy clone etc.
void Mesh::deleteMesh(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Mesh::render() const {
    //todo ??? redo api
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, count);
    glBindVertexArray(0);
}

