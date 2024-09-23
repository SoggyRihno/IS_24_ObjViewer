#pragma once


#include <string>
#include <fstream>
#include <iostream>
#include "glad/glad.h"
#include <utility>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Program {
public:
    unsigned int ID;

    Program(std::string vertexPath, std::string fragmentPath);

    // use/activate the shader
    void use();

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;

    void setMatx4fv(std::string name, glm::mat4 value);
    void set3fv(std::string name, glm::vec3 value);

private:
    static unsigned int CreateShader(unsigned int type, std::string path);
};
