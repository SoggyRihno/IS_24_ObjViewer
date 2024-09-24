#include <fstream>
#include <iostream>
#include <stdexcept>
#include "Program.h"
#include <glm/gtc/type_ptr.hpp>

/*
 *  todo should exceptions be used ? no reason to continue execution if shaders are fucked ?
 *  exceptions are only non-performant if the are thrown but we arent catching them anyways
 */

Program::Program(const std::string &vertPath, const std::string &fragPath) {
    uint32_t vertexShader = createShader(GL_VERTEX_SHADER, vertPath);
    uint32_t fragmentShader = createShader(GL_FRAGMENT_SHADER, fragPath);

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    int success;
    char infoLog[1024];
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 1024, nullptr, infoLog);
        throw std::runtime_error(
                std::string("Error Shader Program Failed Linking\n path :") + vertPath + std::string("\nLog :") +
                std::string(infoLog) + std::string("\n"));
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Program::~Program() { glDeleteProgram(id); }

uint32_t Program::createShader(uint32_t type, const std::string &shaderPath) {
    std::ifstream file(shaderPath, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error(std::string("Failed to open file ") + shaderPath + std::string("\n"));
    }

    std::string contents((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());

    const char *source = contents.c_str();

    uint32_t shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        //no exception because shader error may still function partially and might be helpful for debugging todo might fix program compilation not throwing
        if (type == GL_VERTEX_SHADER) {
            printf("Vertex shader compilation error\n%s\n", infoLog);
        } else {
            printf("Vertex fragment compilation error\n%s\n", infoLog);
        }
    }
    return shader;
}

void Program::use() const {
    glUseProgram(id);
}

void Program::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int) value);
}

void Program::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Program::setUInt(const std::string &name, uint32_t value) const {
    glUniform1ui(glGetUniformLocation(id, name.c_str()), value);
}

void Program::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Program::set3fv(const std::string &name, glm::vec3 value) const {
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
}

void Program::setMatx4fv(const std::string &name, glm::mat4 value) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()),
                       1, GL_FALSE,
                       glm::value_ptr(value)
    );
}

void Program::bindTexture(const std::string &name, uint32_t texture, uint32_t textureUnit) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);  // Activate the texture unit
    glBindTexture(GL_TEXTURE_2D, texture);
}

