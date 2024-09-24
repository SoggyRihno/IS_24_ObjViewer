
#include "Mesh.h"

Mesh::Mesh(ProgramType programType, const std::vector<float> &vertices, Material material)
        : programType(programType), material(material) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    uint32_t stride = Loader::getStride(programType);
    assert(vertices.size() % stride == 0);
    count = vertices.size() / stride;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    //todo code is a little awkward ? maybe rewrite
    if (stride > 3) {
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    if (stride > 6) {
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    //todo might be the problem ???
    /*
     *     glDeleteVertexArrays(1, &VAO);
     *     glDeleteBuffers(1, &VBO);
     */
}

void Mesh::render() const {
    //todo ??? redo api
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, count);
    glBindVertexArray(0);
}
