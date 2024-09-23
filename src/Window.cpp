#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include "Window.h"


Window::Window(int height, int width) :
        height(height), width(width) {

    if (!glfwInit()) {
        throw std::runtime_error("Could not init GLFW");
    }

    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, 1);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    window = glfwCreateWindow(width, height, "WINDOW_TITLE", nullptr, nullptr);

    if (!window) {
        throw std::runtime_error("Could not create Window");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::error_callback(int error, const char *description) {
    printf("Error: %s\n", description);
}

void Window::processInput(GLFWwindow *window) {

}

void Window::mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {

}

void Window::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
