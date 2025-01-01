#include <thread>
#include <filesystem>
#include "Window.h"
#include "Program.h"
#include "Loader.h"

std::string getFolderFromPath(const std::string &filePath) {
    std::filesystem::path path(filePath);

    // Check if the given path is a file or directory
    if (std::filesystem::is_regular_file(path)) {
        return path.parent_path().string(); // Get parent folder of file
    }
        // If it's a directory, return the directory itself
    else if (std::filesystem::is_directory(path)) {
        return path.string(); // Directory as it is
    }

    // If no folder found (e.g., input is just a filename with no directory)
    return "";
}

int main(int argc, const char *argv[]) {
    Window &window = Window::getInstance();
    Loader &loader = Loader::getInstance();

    if (argc != 2) {
        printf("Improper Arguments, only provide the path to the .obj");
        return -1;
    }
    std::string file = argv[1];
    std::string folder = getFolderFromPath(file);

    if (!file.ends_with(".obj")) {
        printf("Invalid file type, must be .obj");
        return -1;
    }

    if (!loader.loadObjFromFile(file, folder)) {
        printf("Failed to load %s", file.c_str());
        return -1;
    }

    float deltaTime;
    float lastFrame = 0.0f;

    while (!window.shouldClose()) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        float sleep = (1.0f / 60.0f) - deltaTime;
        if (sleep > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(int(sleep * 1000)));
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        const auto &meshes = loader.getMeshes();
        for (const auto &object: loader.getObjects()) {
            for (const auto &index: object.meshIndices) {
                const auto &mesh = meshes.at(index);
                const auto &program = mesh.getProgram();
                program.use();

                window.updateUniforms(deltaTime, program);
                program.setMatx4fv("model", glm::mat4(1.0f));
                mesh.material.bind(program);
                mesh.render();
            }

        }

        window.update();
    }
}


