#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"


//todo make config options better, maybe config file

class Window {
public:
    static const Window &getInstance(){
        static Window instance;
        return instance;
    };

    void update() const{
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

private:
    GLFWwindow *window{};
    int width;
    int height;

    Window(int height = 900, int width = 1600);

    ~Window();

    static void error_callback(int error, const char *description);

    static void processInput(GLFWwindow *window);

    static void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
};