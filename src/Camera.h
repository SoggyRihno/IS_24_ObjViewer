#pragma once

#include "glm/vec3.hpp"
#include "Program.h"

class Camera {
public:
    //should be looking at center of screen at start
    Camera(float width, float height) : height(height), width(width), lastX(width / 2.0f), lastY(height / 2.0f) {}

    void updateMouse(float x, float y);

    void forward(float deltaTime);

    void back(float deltaTime);

    void left(float deltaTime);

    void right(float deltaTime);

    void sprint(float mult);

    void stopSprint();

    void updateUniforms(Program &program) const;

    void print() const;

private:
    glm::vec3 cameraPos{0.0f, 0.0f, 3.0f};
    glm::vec3 cameraFront{0.0f, 0.0f, -1.0f};
    glm::vec3 cameraUp{0.0f, 1.0f, 0.0f};
    float width;
    float height;
    float lastX;
    float lastY;

    float sensitivity = 0.01f;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float fov = 45.0f;
    float speed = 25;
    float sprintMultiplier = 1;
};