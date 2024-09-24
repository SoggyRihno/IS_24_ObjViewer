#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Window.h"
#include "Program.h"
#include "Mesh.h"
#include "Camera.h"

int main(int argc, const char *argv[]) {
    const Window& window = Window::getInstance();
    Program program("shaders/simple.vert", "shaders/simple.frag");

    std::vector<float> verts = {
            // Front face (z = 0.5f)
            -0.5f, -0.5f, 0.5f,  // Bottom-left
            0.5f, -0.5f, 0.5f,  // Bottom-right
            0.5f, 0.5f, 0.5f,  // Top-right
            0.5f, 0.5f, 0.5f,  // Top-right
            -0.5f, 0.5f, 0.5f,  // Top-left
            -0.5f, -0.5f, 0.5f,  // Bottom-left

            // Back face (z = -0.5f)
            -0.5f, -0.5f, -0.5f,  // Bottom-left
            0.5f, -0.5f, -0.5f,  // Bottom-right
            0.5f, 0.5f, -0.5f,  // Top-right
            0.5f, 0.5f, -0.5f,  // Top-right
            -0.5f, 0.5f, -0.5f,  // Top-left
            -0.5f, -0.5f, -0.5f,  // Bottom-left

            // Left face (x = -0.5f)
            -0.5f, 0.5f, 0.5f,  // Top-right
            -0.5f, 0.5f, -0.5f,  // Top-left
            -0.5f, -0.5f, -0.5f,  // Bottom-left
            -0.5f, -0.5f, -0.5f,  // Bottom-left
            -0.5f, -0.5f, 0.5f,  // Bottom-right
            -0.5f, 0.5f, 0.5f,  // Top-right

            // Right face (x = 0.5f)
            0.5f, 0.5f, 0.5f,  // Top-left
            0.5f, 0.5f, -0.5f,  // Top-right
            0.5f, -0.5f, -0.5f,  // Bottom-right
            0.5f, -0.5f, -0.5f,  // Bottom-right
            0.5f, -0.5f, 0.5f,  // Bottom-left
            0.5f, 0.5f, 0.5f,  // Top-left

            // Top face (y = 0.5f)
            -0.5f, 0.5f, 0.5f,  // Top-left
            0.5f, 0.5f, 0.5f,  // Top-right
            0.5f, 0.5f, -0.5f,  // Bottom-right
            0.5f, 0.5f, -0.5f,  // Bottom-right
            -0.5f, 0.5f, -0.5f,  // Bottom-left
            -0.5f, 0.5f, 0.5f,  // Top-left

            // Bottom face (y = -0.5f)
            -0.5f, -0.5f, 0.5f,  // Top-left
            0.5f, -0.5f, 0.5f,  // Top-right
            0.5f, -0.5f, -0.5f,  // Bottom-right
            0.5f, -0.5f, -0.5f,  // Bottom-right
            -0.5f, -0.5f, -0.5f,  // Bottom-left
            -0.5f, -0.5f, 0.5f   // Top-left
    };


    Mesh mesh(ProgramType::VERT, verts);

    program.use();
    Camera camera(1600, 900);

    float deltaTime;
    float lastFrame = 0.0f;
    size_t frame = 0;

    while (!window.shouldClose()) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        float sleep = (1.0f / 60.0f) - deltaTime;
        if (sleep > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(int(sleep * 1000)));
        }


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        window.updateCamera(camera, deltaTime);
        camera.updateUniforms(program);
        program.setMatx4fv("model", glm::mat4(1.0f));


        mesh.render();

        window.update();
        frame++;
        if (frame % 60 == 0)
            camera.print();
    }
}


