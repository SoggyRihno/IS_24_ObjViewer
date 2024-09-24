#pragma once

#include "Program.h"

enum class ProgramType {
    VERT,
    VERT_NORM,
    VERT_NORM_TEX,
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

    std::vector <Program> programs;
    //todo texture wrapper ?
    std::unordered_map <std::string, uint32_t> textures;

    Loader();

    ~Loader();
};