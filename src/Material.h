#pragma once

#include <glm/vec3.hpp>
#include "Program.h"

enum USING_TEXTURE{
    USE_AMBIENT_TEXTURE,
    USE_DIFFUSE_TEXTURE,
    USE_SPECULAR_TEXTURE,
    USE_EMISSIVE_TEXTURE,
    USE_SPECULAR_HIGHLIGHT_TEXTURE,
    USE_BUMP_TEXTURE,
};


struct Material {
    glm::vec3 Ka = {0.2f, 0.2f, 0.2f};      //Ambient color of the material
    glm::vec3 Kd = {0.8f, 0.8f, 0.8f};      //Diffuse color of the material
    glm::vec3 Ks = {0.0f, 0.0f, 0.0f};      //Specular color of the material
    glm::vec3 Ke = {0.0f, 0.0f, 0.0f};      //Emissive color of the material.

    float Ns = 10.0f;                       //Specular exponent (shininess) of the material.
    float d = 1.0f;                         //Transparency of the material. Tr = 1-d


    int map_Ka = -1;                        //Ambient texture
    int map_Kd = -1;                        //Diffuse texture
    int map_Ks = -1;                        //Specular texture
    int map_Ke = -1;                        //Emissive texture
    int map_Ns = -1;                        //Specular highlight texture
    int map_bump = -1;                      //Bump map (normal map)

    //All textures are loaded into opengl on load, so we only store opengl Texture ids

    Material() = default;

    ~Material() = default;
    void bind(const Program &program) const;
};