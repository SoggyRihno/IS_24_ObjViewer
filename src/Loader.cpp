#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <tiny_obj_loader.h>
#include <stb_image.h>
#include <filesystem>
#include "Loader.h"

uint32_t Loader::getStride(ProgramType type) {
    switch (type) {
        case ProgramType::VERT:
            return 3;
        case ProgramType::VERT_NORM:
            return 6;
        case ProgramType::VERT_NORM_TEX:
            return 8;
    }
}

Loader::Loader() {
    //todo make a more robust system, and use config
    programs.emplace_back("shaders/Vert.vert", "shaders/Vert.frag");
    programs.emplace_back("shaders/VertNorm.vert", "shaders/VertNorm.frag");
    programs.emplace_back("shaders/VertNormTex.vert", "shaders/VertNormTex.frag");
}

Loader::~Loader() {
    for (const auto &item: textures) {
        glDeleteTextures(1, &item.second);
    }
}

/*
 #define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <filesystem>
#include <tiny_obj_loader.h>
#include <stb_image.h>
#include "Loader.h"

Loader::Loader(const std::string &file, const std::string &folder) : file(file), folder(folder) {
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig config;
    config.mtl_search_path = folder;

    if (!reader.ParseFromFile(file, config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
            exit(-1); //todo panic on loader error
        }
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }


    std::vector<Material> materials;
    materials.emplace_back();
    int counter = 1;
    const auto &mats = reader.GetMaterials();
    for (const auto &mat: mats) {
        printf("Loading Material %d/%zu : %s \n", counter++, mats.size(), mat.name.c_str());


        Material material;
        material.Ka = {mat.ambient[0], mat.ambient[1], mat.ambient[2]};
        material.Kd = {mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]};
        material.Ks = {mat.specular[0], mat.specular[1], mat.specular[2]};
        material.Ke = {mat.emission[0], mat.emission[1], mat.emission[2]};
        material.Ns = mat.shininess;
        material.d = mat.dissolve;
        material.map_Ka = getOrLoadTexture(mat.ambient_texname);
        material.map_Kd = getOrLoadTexture(mat.diffuse_texname);
        material.map_Ks = getOrLoadTexture(mat.specular_texname);
        material.map_Ke = getOrLoadTexture(mat.emissive_texname);
        material.map_Ns = getOrLoadTexture(mat.specular_highlight_texname);
        material.map_bump = getOrLoadTexture(mat.bump_texname);

        materials.push_back(material);
    }

    //todo extract into methods
    std::vector<std::vector<float>> vertByMat(materials.size() + 1, std::vector<float>());
    counter = 1;
    const tinyobj::attrib_t &attrib = reader.GetAttrib();
    const std::vector<tinyobj::shape_t> &shapes = reader.GetShapes();
    for (const auto &shape: reader.GetShapes()) {
        tinyobj::mesh_t mesh = shape.mesh;
        printf("Loading Shape %d/%zu : %s \n\tFaces : %zu\n", counter++, shapes.size(), shape.name.c_str(),
               mesh.num_face_vertices.size());

        int index_counter = 0;
        for (size_t i = 0; i < shape.mesh.num_face_vertices.size(); i++) {
            //defaults to -1 add 1 to make defualt 0
            int material = shape.mesh.material_ids[i] + 1;

            //todo fix stop passing
            for (int j = 0; j < 3; j++) {
                tinyobj::index_t index = shape.mesh.indices[index_counter];

                vertByMat[material].push_back(attrib.vertices[index.vertex_index * 3]);
                vertByMat[material].push_back(attrib.vertices[(index.vertex_index * 3) + 1]);
                vertByMat[material].push_back(attrib.vertices[(index.vertex_index * 3) + 2]);


                if (index.normal_index >= 0) {
                    vertByMat[material].push_back(attrib.normals[3 * size_t(index.normal_index)]);
                    vertByMat[material].push_back(attrib.normals[3 * size_t(index.normal_index) + 1]);
                    vertByMat[material].push_back(attrib.normals[3 * size_t(index.normal_index) + 2]);
                } else {
                    vertByMat[material].push_back(0);
                    vertByMat[material].push_back(0);
                    vertByMat[material].push_back(0);
                }

                if (index.texcoord_index >= 0) {
                    vertByMat[material].push_back(attrib.texcoords[2 * size_t(index.texcoord_index)]);
                    vertByMat[material].push_back(attrib.texcoords[2 * size_t(index.texcoord_index) + 1]);
                } else {
                    vertByMat[material].push_back(0);
                    vertByMat[material].push_back(0);
                }

                index_counter++;
            }
        }
    }

    for (int i = 0; i < vertByMat.size(); ++i) {
        if (!vertByMat[i].empty()){
            meshes.emplace_back(materials[i],vertByMat[i]);
        }
    }
}

Loader::~Loader() {
    for (const auto &item: textures) {
        glDeleteTextures(1, &item.second);
    }
}

const std::vector<Mesh>& Loader::getMeshes() const{
    return meshes;
}





 */


bool Loader::loadObjFromFile(const std::string &file, const std::string &folder) {
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig config;
    config.mtl_search_path = folder;

    if (!reader.ParseFromFile(file, config)) {
        if (!reader.Error().empty()) {
            throw new std::runtime_error("TinyObjReader: " + reader.Error());
        }
    }

    if (!reader.Warning().empty()) {
        printf("TinyObjReader: %s\n", reader.Warning().c_str());
    }

    //when we access materials by index, we need to adjust index after adding to a list with elements
    auto materialOffSet = materials.size();
    const auto &mats = reader.GetMaterials();
    for (size_t i = 0; i < mats.size(); i++) {
        const auto &mat = mats.at(i);
        printf("Loading Material : %zu/%zu %s \n", i, mats.size(), mat.name.c_str());

        Material material(mat.name);
        material.Ka = {mat.ambient[0], mat.ambient[1], mat.ambient[2]};
        material.Kd = {mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]};
        material.Ks = {mat.specular[0], mat.specular[1], mat.specular[2]};
        material.Ke = {mat.emission[0], mat.emission[1], mat.emission[2]};
        material.Ns = mat.shininess;
        material.d = mat.dissolve;
        material.map_Ka = getOrLoadTexture(mat.ambient_texname, folder);
        material.map_Kd = getOrLoadTexture(mat.diffuse_texname, folder);
        material.map_Ks = getOrLoadTexture(mat.specular_texname, folder);
        material.map_Ke = getOrLoadTexture(mat.emissive_texname, folder);
        material.map_Ns = getOrLoadTexture(mat.specular_highlight_texname, folder);
        material.map_bump = getOrLoadTexture(mat.bump_texname, folder);

        materials.push_back(material);
    }

    const auto &attrib = reader.GetAttrib();
    const auto &shapes = reader.GetShapes();




    for (size_t i = 0; i < shapes.size(); i++) {
        const auto &shape = shapes.at(i);
        auto mesh = shape.mesh;
        printf("Loading Shape %zu/%zu : %s\n", i, shapes.size(), shape.name.c_str());

        //each shape contains sub shapes with different materials
        // (ex. pencil has tip, shaft and eraser. all parts of a pencil but rendered differently)
        std::map<FaceType, std::vector<float>> vertsByFaceType;

        int index_counter = 0;
        for (size_t j = 0; j < mesh.num_face_vertices.size(); j++) {
            //we assume every face is a triangle for simplicity
            assert(mesh.num_face_vertices[j] == 3);

            //recalculate index, default material is at index 0 otherwise it is at the end of materials
            uint32_t materialId = mesh.material_ids[j] == -1 ? 0 : mesh.material_ids[j] + materialOffSet + 1;
            FaceType faceType(materialId);

            std::vector<float> vertices;
            for (int k = 0; k < 3; k++) {
                tinyobj::index_t index = shape.mesh.indices[index_counter];

                vertices.push_back(attrib.vertices[index.vertex_index * 3]);
                vertices.push_back(attrib.vertices[(index.vertex_index * 3) + 1]);
                vertices.push_back(attrib.vertices[(index.vertex_index * 3) + 2]);


                if (index.normal_index > -1) {
                    faceType.programType = ProgramType::VERT_NORM;
                    vertices.push_back(attrib.normals[3 * size_t(index.normal_index)]);
                    vertices.push_back(attrib.normals[3 * size_t(index.normal_index) + 1]);
                    vertices.push_back(attrib.normals[3 * size_t(index.normal_index) + 2]);
                }

                if (index.texcoord_index > -1) {
                    faceType.programType = ProgramType::VERT_NORM_TEX;
                    vertices.push_back(attrib.texcoords[2 * size_t(index.texcoord_index)]);
                    vertices.push_back(attrib.texcoords[2 * size_t(index.texcoord_index) + 1]);
                }
                index_counter++;
            }
            //make sure all vertices match
            assert(vertices.size() == getStride(faceType.programType) * 3);
            auto &vec = vertsByFaceType[faceType];
            vec.insert(vec.end(), vertices.begin(), vertices.end());
        }

    }

    return true;
}

GLuint Loader::loadTexture(const std::string &path) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    int width, height, bpp;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &bpp, 4);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        if (!std::filesystem::exists(path)) {
            printf("Could not find image at : %s", path.c_str());
        } else {
            printf("Failed to load texture at : %s", path.c_str());
        }
        return -1;
    }
    stbi_image_free(data);
    return texture;
}

int Loader::getOrLoadTexture(const std::string &tex_name, const std::string &folder) {
    if (tex_name.empty())
        return -1;
    std::string path = folder + tex_name;
    if (textures.find(tex_name) == textures.end()) {
        textures[tex_name] = loadTexture(tex_name);
    }
    return (int) textures.at(tex_name);
}