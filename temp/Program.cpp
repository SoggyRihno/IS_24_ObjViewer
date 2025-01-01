

#include "Program.h"

Program::Program(std::string vertexPath, std::string fragmentPath) {
    unsigned int vertexShader = CreateShader(GL_VERTEX_SHADER, std::move(vertexPath));
    unsigned int fragmentShader = CreateShader(GL_FRAGMENT_SHADER, std::move(fragmentPath));

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM_LINKING_FAILED of type:\n" << infoLog
                  << "\n -- --------------------------------------------------- -- " << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Program::use() {
    glUseProgram(ID);
}

void Program::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}

void Program::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Program::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Program::setMatx4fv(std::string name, glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),
                       1, GL_FALSE,
                       glm::value_ptr(value)
    );
}

void Program::set3fv(std::string name, glm::vec3 value) {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

unsigned int Program::CreateShader(unsigned int type, std::string path) {
    std::ifstream in(path);
    std::string contents((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());

    const char *source = contents.c_str();
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::cout << "ERROR::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "_SHADER_COMPILATION_ERROR\n"
                  << infoLog
                  << "\n -- --------------------------------------------------- -- " << std::endl;
    }
    return shader;
}