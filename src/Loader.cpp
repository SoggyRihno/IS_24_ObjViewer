#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <tiny_obj_loader.h>
#include <stb_image.h>
#include <filesystem>
#include <glm/glm.hpp>
#include "Loader.h"


Loader::Loader() {
    //todo make a more robust system, and use config
    programs.emplace_back("shaders/sparse.vert", "shaders/sparse.frag");
    materials.emplace_back();
}

Loader::~Loader() {
    for (const auto &item: textures) {
        glDeleteTextures(1, &item.second);
    }
    for (auto &mesh: meshes) {
        mesh.deleteMesh();
    }
    for (auto &program: programs) {
        program.deleteProgram();
    }
}

glm::vec3 computeFallbackNormal(const tinyobj::attrib_t &attrib,
                                const tinyobj::mesh_t &mesh,
                                int baseIndex) {
    const tinyobj::index_t &idx0 = mesh.indices[baseIndex];
    const tinyobj::index_t &idx1 = mesh.indices[baseIndex + 1];
    const tinyobj::index_t &idx2 = mesh.indices[baseIndex + 2];

    auto getVertex = [&](int vertexIndex) {
        return glm::vec3{
                attrib.vertices[3 * vertexIndex],
                attrib.vertices[3 * vertexIndex + 1],
                attrib.vertices[3 * vertexIndex + 2]
        };
    };

    glm::vec3 a = getVertex(idx0.vertex_index);
    glm::vec3 b = getVertex(idx1.vertex_index);
    glm::vec3 c = getVertex(idx2.vertex_index);

    return glm::normalize(glm::cross(b - a, c - a));
}

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
        printf("Loading Material : %zu/%zu %s \n", i + 1, mats.size(), mat.name.c_str());

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
        printf("Loading Object %zu/%zu : %s\n", i + 1, shapes.size(), shape.name.c_str());

        //each shape contains sub shapes with different materials
        // (ex. pencil has tip, shaft and eraser. all parts of a pencil but rendered differently)
        std::map<FaceType, std::vector<float>> vertsByFaceType;

        int index_counter = 0;
        for (size_t j = 0; j < mesh.num_face_vertices.size(); j++) {
            //we assume every face is a triangle for simplicity
            assert(mesh.num_face_vertices[j] == 3);

            //recalculate index, default material is at index 0 otherwise it is at the end of materials
            uint32_t materialId = mesh.material_ids[j] == -1 ? 0 : mesh.material_ids[j] + materialOffSet;
            FaceType faceType(materialId);

            std::vector<float> vertices;
            for (int k = 0; k < 3; k++) {
                tinyobj::index_t index = shape.mesh.indices[index_counter];
                vertices.push_back(attrib.vertices[index.vertex_index * 3]);
                vertices.push_back(attrib.vertices[index.vertex_index * 3 + 1]);
                vertices.push_back(attrib.vertices[index.vertex_index * 3 + 2]);

                if (index.normal_index > -1) {
                    vertices.push_back(attrib.normals[3 * index.normal_index]);
                    vertices.push_back(attrib.normals[3 * index.normal_index + 1]);
                    vertices.push_back(attrib.normals[3 * index.normal_index + 2]);
                } else {
                    glm::vec3 normal = computeFallbackNormal(attrib, mesh, index_counter - k);
                    vertices.insert(vertices.end(), {normal.x, normal.y, normal.z});
                }

                if (index.texcoord_index > -1) {
                    vertices.push_back(attrib.texcoords[2 * size_t(index.texcoord_index)]);
                    vertices.push_back(attrib.texcoords[2 * size_t(index.texcoord_index) + 1]);
                } else {
                    vertices.insert(vertices.end(), {0, 0});
                }
                index_counter++;
            }
            //make sure all vertices match
            assert(vertices.size() == STRIDE * 3);

            auto &vec = vertsByFaceType[faceType];
            vec.insert(vec.end(), vertices.begin(), vertices.end());
        }

        objects.emplace_back(shape.name);
        auto &indices = objects.at(objects.size() - 1).meshIndices;
        for (const auto &pair: vertsByFaceType) {
            indices.push_back(createMesh(pair.first, pair.second));  // Store references to the meshes for this object.
        }

    }

    return true;
}

size_t Loader::createMesh(FaceType f, std::vector<float> vertices) {
    const auto &program = getProgram();
    const auto &material = getMaterial(f.materialIndex);
    //todo rip smooth shading
    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    assert(vertices.size() % STRIDE == 0);
    GLsizei count = vertices.size() / STRIDE;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, STRIDE * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    meshes.emplace_back(VAO, VBO, count, program, material);
    return meshes.size() - 1;
}

GLuint Loader::loadTexture(const std::string &path) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture wrapping and filtering options
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
            fprintf(stderr, "Error: Failed to load texture at: %s\n", path.c_str());
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        return -1; // Error indicator
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture to prevent accidental modification
    return texture;
}


int Loader::getOrLoadTexture(const std::string &tex_name, const std::string &folder) {
    if (tex_name.empty())
        return -1;
    if (textures.find(tex_name) == textures.end()) {
        textures[tex_name] = loadTexture(folder + tex_name);
    }
    return (int) textures.at(tex_name);
}

const Program &Loader::getProgram() {
    return programs.at(0);
}

const Material &Loader::getMaterial(size_t index) {
    return materials.at(index);
}

const std::vector<Object> &Loader::getObjects() {
    return objects;
}

const std::vector<Mesh> &Loader::getMeshes() {
    return meshes;
}