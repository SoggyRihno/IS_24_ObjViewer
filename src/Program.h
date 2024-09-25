#pragma once

#include <string>
#include <glad/glad.h>
#include "glm/vec3.hpp"
#include "glm/fwd.hpp"


class Program {

public:
    Program(const std::string &vertPath, const std::string &fragPath);

    void use() const;

    void deleteProgram();

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setUInt(const std::string &name, uint32_t value) const;

    void setFloat(const std::string &name, float value) const;

    void set3fv(const std::string &name, glm::vec3 value) const;

    void setMatx4fv(const std::string &name, glm::mat4 value) const;

    void bindTexture(const std::string &name, uint32_t texture, uint32_t textureUnit = 0) const;

private:
    uint32_t id;

    static uint32_t createShader(uint32_t type, const std::string &shaderPath);

};