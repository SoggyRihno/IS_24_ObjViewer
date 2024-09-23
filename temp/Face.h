#pragma once

#include <vector>

struct Face {
    bool normal, texture;
    std::vector<float> vertices;
};