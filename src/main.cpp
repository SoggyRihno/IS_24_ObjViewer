#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Window.h"
#include "Program.h"
#include "Mesh.h"
#include "Camera.h"
#include "Loader.h"

int main(int argc, const char *argv[]) {
    const Window &window = Window::getInstance();
    Loader &loader = Loader::getInstance();
    Camera camera(1600, 900);


    if (!loader.loadObjFromFile("conference.obj")) {
        printf("Loading failed");
        return -1;
    }


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

        window.updateCamera(camera, deltaTime);
        const auto& meshes = loader.getMeshes();
        for (const auto &object: loader.getObjects()){
            for (const auto &index: object.meshIndices){
                const auto & mesh = meshes.at(index);
                const auto & program = mesh.getProgram();
                program.use();

                camera.updateUniforms(program);
                program.setMatx4fv("model", glm::mat4(1.0f));
                mesh.render();
            }

        }

        window.update();
        frame++;
        if (frame % 60 == 0)
            camera.print();
    }
}


