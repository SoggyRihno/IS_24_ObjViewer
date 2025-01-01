#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "Program.h"

struct Light {
    glm::vec3 position;
    float padding1{0};  // Needed for std140 layout
    glm::vec3 color{255,255,255};
    float intensity{.05};

    Light(glm::vec3 position) : position(position){};
};

class LightingManager {
public:

    LightingManager();
    ~LightingManager();

    void nextLight();
    void previousLight();

    void nextLightReset();
    void previousLightReset();

    void setCurrentLightPosition(glm::vec3 position);
    void incrementCurrentLightColor(glm::vec3 diff, float deltaTime);
    void incrementCurrentLightIntensity(float diff, float deltaTime);

    void spawnLight(glm::vec3 position);
    void deleteCurrentLight();

    void spawnLightReset();
    void deleteCurrentLightReset();


    void updateUniforms();

    void debugCurrentLight() const;

private:
    std::vector<Light> lights{};
    size_t index = 0;
    GLuint uboLights = -1;
    static constexpr size_t MAX_LIGHTS = 15;


    bool lightAdd = true;
    bool lightDelete = true;

    bool lightNext = true;
    bool lightPrevious = true;

    const glm::vec3 colorIncrementer{1,1,1};
    const float intensityIncrementer{.01};
};