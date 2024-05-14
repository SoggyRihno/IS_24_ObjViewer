//
// Created by Ian on 2/14/2024.
//

#ifndef OPENGLTEST_TEXTURE_H
#define OPENGLTEST_TEXTURE_H

#include <string>
#include <glad/glad.h>
#include <iostream>
#include <filesystem>
#include <stb_image.h>

class Texture {
public:
    Texture();
    Texture(std::string &path);

    void Bind() const;

    std::string filepath;
private:
    unsigned int ID;
};


#endif //OPENGLTEST_TEXTURE_H