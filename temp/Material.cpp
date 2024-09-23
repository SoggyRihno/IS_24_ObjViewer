//
// Created by Ian on 5/12/2024.
//

#include "Material.h"

Material::Material() {
    Ka = glm::vec3 (1.0f, 1.0f, 1.0f);
    Kd = glm::vec3 (0.64f, 0.64f, 0.64f);
    Ks = glm::vec3 (0.5f, 0.5f, 0.5f);
    Ns = 96.078431f;
}
