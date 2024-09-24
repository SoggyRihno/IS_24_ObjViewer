
#include "Loader.h"

Loader::Loader() {
    //todo make a more robust system, and use config
    programs.emplace_back("shaders/Vert.vert", "shaders/Vert.frag");
    programs.emplace_back("shaders/VertNorm.vert", "shaders/VertNorm.frag");
    programs.emplace_back("shaders/VertNormTex.vert", "shaders/VertNormTex.frag");
}

Loader::~Loader() {
    for (const auto &item: textures) {
        glDeleteTextures(1, &item.second);
    }

}

bool Loader::loadObjFromFile(const std::string &file, const std::string &folder) {
    //todo finsih
    return true;
}

uint32_t Loader::getStride(ProgramType type) {
    switch (type) {
        case ProgramType::VERT:
            return 3;
        case ProgramType::VERT_NORM:
            return 6;
        case ProgramType::VERT_NORM_TEX:
            return 8;
    }
}
