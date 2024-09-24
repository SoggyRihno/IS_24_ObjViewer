#pragma once

#include "Program.h"
#include "Material.h"

enum class ProgramType {
    VERT,
    VERT_NORM,
    VERT_NORM_TEX,
};

enum class ShadingType {
    Smooth,
    Normal,
};

struct FaceType {
    uint32_t materialIndex = 0;
    ProgramType programType = ProgramType::VERT;
    ShadingType shadingType = ShadingType::Smooth;

    FaceType() =default;
    ~FaceType() =default;
};

//todo set config file void setConfig(const std::string& file);
class Loader {
public:
    Loader &getInstance() {
        static Loader instance;
        return instance;
    }

    bool loadObjFromFile(const std::string &file, const std::string &folder = "");

    static uint32_t getStride(ProgramType type);

private:
    std::vector<Program> programs;
    std::unordered_map<std::string, GLuint> textures;
    std::vector<Material> materials;

    Loader();

    ~Loader();

    GLuint loadTexture(const std::string &path);

    int getOrLoadTexture(const std::string &tex_name, const std::string &folder);
};