//
// Created by Ian on 5/12/2024.
//

#ifndef OPENGLTEST_FACE_H
#define OPENGLTEST_FACE_H


#include <vector>

struct Face {
    bool normal, texture;
    std::vector<float> vertices;
};


#endif //OPENGLTEST_FACE_H
