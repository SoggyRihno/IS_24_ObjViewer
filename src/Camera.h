#pragma once

#include "glm/vec3.hpp"
#include "Program.h"

class Camera {
public:
    //should be looking at center of screen at start
    Camera(float height, float width) : height(height), width(width), lastX(width / 2.0f), lastY(height / 2.0f) {}

    void updateMouse(float x, float y);

    void forward(float deltaTime);

    void back(float deltaTime);

    void left(float deltaTime);

    void right(float deltaTime);

    void sprint(float mult = 1);

    void updateUniforms(const Program &program) const;

    void print() const;

    glm::vec3 getPosition();

private:
    glm::vec3 cameraPos{0.0f, 0.0f, 0.0f};
    glm::vec3 cameraFront{0.0f, 0.0f, -1.0f};
    glm::vec3 cameraUp{0.0f, 1.0f, 0.0f};
    float width;
    float height;
    float lastX;
    float lastY;

    float sensitivity = 0.1f;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float fov = 45.0f;
    float speed = .05;
    float sprintMultiplier = 1;
};