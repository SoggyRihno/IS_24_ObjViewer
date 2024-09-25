#pragma once

#include "Program.h"
#include "Material.h"
#include "Mesh.h"

enum class ProgramType {
    VERT,
    VERT_NORM,
    VERT_NORM_TEX,
};

//todo smooth shading is ignored
enum class ShadingType {
    Smooth,
    Normal,
};

struct FaceType {
    uint32_t materialIndex = 0;
    ProgramType programType = ProgramType::VERT;
    ShadingType shadingType = ShadingType::Smooth;

    FaceType(uint32_t i): materialIndex(i) {};
    ~FaceType() =default;

    bool operator<(const FaceType& other) const {
        if (materialIndex != other.materialIndex) {
            return materialIndex < other.materialIndex;
        }
        if (shadingType != other.shadingType) {
            return shadingType < other.shadingType;
        }
        return programType < other.programType;
    }
};

struct Object {
    const std::string name;
    std::vector<size_t> meshIndices{};

    Object(std::string name) : name(std::move(name)){};
};


//todo set config file void setConfig(const std::string& file);
class Loader {
public:
    static Loader &getInstance() {
        static Loader instance;
        return instance;
    }

    bool loadObjFromFile(const std::string &file, const std::string &folder = "");

    static uint32_t getStride(ProgramType type);

    const Program& getProgram(ProgramType p = ProgramType::VERT);

    const std::vector<Mesh>& getMeshes();

    const std::vector<Object>& getObjects();

    size_t createMesh(FaceType f, std::vector<float> vertices);

private:
    std::vector<Program> programs;
    std::unordered_map<std::string, GLuint> textures;
    std::vector<Material> materials;
    std::vector<Object> objects;
    std::vector<Mesh> meshes;

    Loader();

    ~Loader();

    GLuint loadTexture(const std::string &path);

    int getOrLoadTexture(const std::string &tex_name, const std::string &folder);


    const Material& getMaterial(size_t index = 0);
};