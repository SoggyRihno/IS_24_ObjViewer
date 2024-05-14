//
// Created by Ian on 5/12/2024.
//

#include "RenderGroup.h"

static const int stride = 8;

RenderGroup::RenderGroup(std::vector<Face> &faces, Material *material) {
    color.r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 1.0f);
    color.g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 1.0f);
    color.b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 1.0f);
    count = faces.size() * 3;
    this->material = material;

    std::vector<float> vertices;
    for (const auto &face: faces) {

        for (int j = 0; j < 3; j++) {
            vertices.push_back(face.vertices.at(j * stride));
            vertices.push_back(face.vertices.at(j * stride + 1));
            vertices.push_back(face.vertices.at(j * stride + 2));

            vertices.push_back(face.vertices.at(j * stride + 3));
            vertices.push_back(face.vertices.at(j * stride + 4));
            vertices.push_back(face.vertices.at(j * stride + 5));

            vertices.push_back(face.vertices.at(j * stride + 6));
            vertices.push_back(face.vertices.at(j * stride + 7));
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void RenderGroup::render(Program &program) const {
    glBindVertexArray(VAO);
    program.set3fv("matColor", color);

    program.set3fv("material.Ka", material->Ka);
    program.set3fv("material.Kd", material->Kd);
    program.set3fv("material.Ks", material->Ks);
    program.setFloat("material.Ns", material->Ns);
    program.setInt("hasDiffuse", 0);

    auto name = material->name;
    if (!material->map_Kd.filepath.empty()) {
        program.setInt("hasDiffuse", 1);
        glActiveTexture(GL_TEXTURE0);
        material->map_Kd.Bind();
    }
    glDrawArrays(GL_TRIANGLES, 0, count);
}