#pragma once

#include "Program.h"
#include "Material.h"
#include "Mesh.h"


//todo smooth shading is ignored

const uint32_t STRIDE = 8;

struct FaceType {
    uint32_t materialIndex = 0;

    FaceType(uint32_t i) : materialIndex(i) {};

    ~FaceType() = default;

    bool operator<(const FaceType &other) const {
        return materialIndex < other.materialIndex;
    }
};

struct Object {
    const std::string name;
    std::vector<size_t> meshIndices{};

    Object(std::string name) : name(std::move(name)) {};
};


//todo set config file void setConfig(const std::string& file);
class Loader {
public:
    static Loader &getInstance() {
        static Loader instance;
        return instance;
    }

    bool loadObjFromFile(const std::string &file, const std::string &folder = "");

    //static uint32_t getStride(ProgramType type);

    const Program &getProgram();

    const std::vector<Mesh> &getMeshes();

    const std::vector<Object> &getObjects();

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

    const Material &getMaterial(size_t index = 0);
};