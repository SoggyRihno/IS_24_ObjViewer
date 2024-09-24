#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include "Window.h"


Window::Window(int height, int width) :
        height(height), width(width) {

    if (!glfwInit()) {
        throw std::runtime_error("Could not init GLFW");
    }

    glfwSetErrorCallback([](int error, const char *description) {
        printf("Error: %s\n", description);
    });
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
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *glfWwindow, int width, int height) {
        glViewport(0, 0, width, height);
    });
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}


void Window::update() const{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::updateCamera(Camera &camera, float deltaTime) const{
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.sprint(81);
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.sprint(9);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.forward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.back(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.left(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.right(deltaTime);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    double x, y;
    glfwGetCursorPos(window, &x, &y);
    camera.updateMouse(static_cast<float>(x), static_cast<float>(y));
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

