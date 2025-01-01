
#include "LightingManager.h"
#include "Program.h"
#include <glm/glm.hpp>
#include <iostream>

LightingManager::LightingManager() {
    lights.emplace_back(glm::vec3{0, 0, 0});
}


LightingManager::~LightingManager() {
    glDeleteBuffers(1, &uboLights);
}

void LightingManager::nextLight() {
    if (lightNext && !lights.empty()) {
        index = (index + 1) % lights.size();
        lightNext = false;
    }
}

void LightingManager::nextLightReset() {
    lightNext = true;
}

void LightingManager::previousLight() {
    if (lightPrevious && !lights.empty()) {
        index = (index == 0) ? lights.size() - 1 : index - 1;
        lightPrevious = false;
    }
}

void LightingManager::previousLightReset() {
    lightPrevious = true;
}

void LightingManager::setCurrentLightPosition(glm::vec3 position) {
    lights[index].position = position;
}

void LightingManager::incrementCurrentLightColor(glm::vec3 diff, float deltaTime) {
    lights[index].color = glm::clamp(lights[index].color + colorIncrementer * diff * deltaTime, {0, 0, 0},
                                     {255, 255, 255});

    std::cout << "Updating light color to (" << lights[index].color.x << ", " << lights[index].color.y << ", " << lights[index].color.z << ")\n";

}

void LightingManager::incrementCurrentLightIntensity(float diff, float deltaTime) {
    lights[index].intensity = glm::clamp(lights[index].intensity + (intensityIncrementer * diff * deltaTime), 0.0f, 1.0f);
}

void LightingManager::spawnLight(glm::vec3 position) {
    if (lightAdd) {
        lights.emplace_back(position);
        index = lights.size() - 1;  // Set to the newly added light
        lightAdd = false;
    }
}

void LightingManager::deleteCurrentLight() {
    if (lightDelete && !lights.empty()) {
        lights.erase(lights.begin() + index);
        // Adjust index after deletion
        if (index >= lights.size()) {
            index = lights.empty() ? 0 : lights.size() - 1;
        }
        lightDelete = false;
    }
}

void LightingManager::spawnLightReset() {
    lightAdd = true;
}

void LightingManager::deleteCurrentLightReset() {
    lightDelete = true;
}

void LightingManager::updateUniforms() {
    if (uboLights == -1) {
        glGenBuffers(1, &uboLights);
        glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
        // Add extra padding to ensure alignment
        const GLint alignment = 16; // std140 requires vec3 to align to vec4
        const GLint offsetToLights = (sizeof(int) + (alignment - 1)) & ~(alignment - 1);
        glBufferData(GL_UNIFORM_BUFFER, offsetToLights + (sizeof(Light) * MAX_LIGHTS), nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboLights);
    }

    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);

    // Update number of lights
    int numLights = static_cast<int>(lights.size());
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int), &numLights);

    // Calculate offset to lights array with proper alignment
    const GLint alignment = 16;
    const GLint offsetToLights = (sizeof(int) + (alignment - 1)) & ~(alignment - 1);

    // Update light data with correct offset
    if (!lights.empty()) {
        glBufferSubData(GL_UNIFORM_BUFFER, offsetToLights, sizeof(Light) * lights.size(), lights.data());
    }
}

void LightingManager::debugCurrentLight() const {
    if (!lights.empty() && index < lights.size()) {
        const auto& light = lights[index];
        std::cout << "Current Light [" << index + 1 << "/" << lights.size() << "]:\n"
                  << "Position: (" << light.position.x << ", " << light.position.y << ", " << light.position.z << ")\n"
                  << "Color: (" << light.color.x << ", " << light.color.y << ", " << light.color.z << ")\n"
                  << "Intensity: " << light.intensity << "\n";
    }
}