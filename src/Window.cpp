#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include "Window.h"


Window::Window(int height, int width) : height(height), width(width) {

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}


void Window::update() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::updateUniforms(float deltaTime, const Program &program) {
    updateCamera(deltaTime);
    camera.updateUniforms(program);
    updateLights(deltaTime);
    lightingManager.updateUniforms();
}

void Window::updateCamera(float deltaTime) {
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

void Window::updateLights(float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS) {
        lightingManager.spawnLight(camera.getPosition());
    }
    if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_RELEASE) {
        lightingManager.spawnLightReset();
    }

    if (glfwGetKey(window, GLFW_KEY_APOSTROPHE) == GLFW_PRESS) {
        lightingManager.deleteCurrentLight();
    }
    if (glfwGetKey(window, GLFW_KEY_APOSTROPHE) == GLFW_RELEASE) {
        lightingManager.deleteCurrentLightReset();
    }

    if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_RELEASE)
        lightingManager.nextLightReset();
    if (glfwGetKey(window, GLFW_KEY_APOSTROPHE) == GLFW_RELEASE)
        lightingManager.previousLightReset();

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) { // next
        lightingManager.nextLight();
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) { // previous
        lightingManager.previousLight();
    }

    //set current position
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        lightingManager.setCurrentLightPosition(camera.getPosition());
    }

    // inc/dec color or intensity
    int sign = 0;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        sign++;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        sign--;
    }

    if (sign != 0) {
        glm::vec3 color(0.0f);
        if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)// red
            color.x = sign;
        if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) // green
            color.y = sign;
        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) // blue
            color.z = sign;

        if (color.x == 0 && color.y == 0 && color.z == 0) {
            lightingManager.incrementCurrentLightIntensity(sign, deltaTime);
        } else {
            lightingManager.incrementCurrentLightColor(color, deltaTime);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        lightingManager.debugCurrentLight();
    }
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(window);
}