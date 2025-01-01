#pragma once


#include <string>
#include "glad/glad.h"
#include <iostream>
#include <filesystem>
#include "stb_image.h"

class Texture {
public:
    Texture();
    Texture(std::string &path);

    void Bind() const;

    std::string filepath;
private:
    unsigned int ID;
};