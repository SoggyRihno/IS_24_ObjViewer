#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Camera.h"


//todo make config options better, maybe config file

class Window {
public:
    //this cant be made internal only since it loads glad and needs to be call first

    static const Window &getInstance() {
        static Window instance;
        return instance;
    };

    void updateCamera(Camera &camera, float deltaTime) const;
    void update() const;
    bool shouldClose() const;

private:
    GLFWwindow *window{};
    int width;
    int height;
    float mouseX;
    float mouseY;

    Window(int glfWwindow = 900, int width = 1600);
    ~Window();
};