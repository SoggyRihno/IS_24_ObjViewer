#pragma once
#include <string>
#include <vector>
#include "Mesh.h"

class Object {

public:
    Object(std::string name, const std::vector<const Mesh*> &meshes) : name(std::move(name)), meshes(meshes) {}

    const std::vector<const Mesh*>& getMeshes() const {
        return meshes;
    }

private:
    const std::string name;
    const std::vector<const Mesh*> meshes;
};