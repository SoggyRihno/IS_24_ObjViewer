#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "LightingManager.h"


//todo make config options better, maybe config file

class Window {
public:
    //this cant be made internal only since it loads glad and needs to be call first

    static Window &getInstance() {
        static Window instance;
        return instance;
    };

    void updateUniforms(float deltaTime, const Program &program);
    void update();
    bool shouldClose();

private:
    GLFWwindow *window{};
    Camera camera{900, 1600};
    LightingManager lightingManager{};

    int width;
    int height;


    Window(int glfWwindow = 900, int width = 1600);
    ~Window();

    void updateCamera(float deltaTime);
    void updateLights(float deltaTime);
};