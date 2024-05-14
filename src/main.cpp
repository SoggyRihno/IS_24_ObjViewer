#define GLFW_INCLUDE_NONE
#define TINYOBJLOADER_IMPLEMENTATION

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <tiny_obj_loader.h>
#include <mapbox/earcut.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Program.h"
#include "RenderGroup.h"

static const int WINDOW_WIDTH = 1200;
static const int WINDOW_HEIGHT = 675;

// camera
glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 lightColor = glm::vec3(1, 1, 1);


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int mode = 0;
bool wireFrame = false;


static void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void GetGLInfo() {
    std::cout << glGetString(GL_VENDOR) << std::endl;
    std::cout << glGetString(GL_RENDERER) << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

static void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // right
        lightPos = cameraPos;

    float cameraSpeed = 25 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) // sprint
        cameraSpeed *= 9;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // forward
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //back
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //left
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // right
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // right
        mode = 1;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // right
        mode = 2;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) // right
        mode = 3;
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) // right
        mode = 4;

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) // flat color
        wireFrame = !wireFrame;

}

static void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static GLFWwindow *InitWindow() {
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, 1);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "WINDOW_TITLE", nullptr, nullptr);

    if (!window) {
        fprintf(stderr, "Could not create Window");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    GetGLInfo();
    return window;
}

int main(int argc, char *argv[]) {
    srand(time(0));
    if (!glfwInit()) {
        printf("Could not init GLFW");
        return 1;
    }
    GLFWwindow *window = InitWindow();
    Program *program = new Program("../shaders/vertexShader.vert", "../shaders/fragmentShader.frag");
    program->use();


    std::string file = "../assets/" + std::string(argv[1]);
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "../assets"; // Path to material files

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(file, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    std::vector<Material*> materials;
    Material *default_material = new Material();
    materials.push_back(default_material);

    for (const auto &mat_t: reader.GetMaterials()) {
        Material *material = new Material();

        material->name = mat_t.name;
        material->Ka = glm::vec3(mat_t.ambient[0], mat_t.ambient[1], mat_t.ambient[2]);
        material->Kd = glm::vec3(mat_t.diffuse[0], mat_t.diffuse[1], mat_t.diffuse[2]);
        material->Ks = glm::vec3(mat_t.specular[0], mat_t.specular[1], mat_t.specular[2]);
        material->Ns = float(mat_t.shininess);

        if (!mat_t.diffuse_texname.empty()) {
            std::string path = "../assets/" + mat_t.diffuse_texname;
            material->map_Kd = *new Texture(path);
        }

        if (!mat_t.diffuse_texname.empty()) {
            std::string path = "../assets/" + mat_t.diffuse_texname;
            material->map_Kd = *new Texture(path);
        }

        materials.push_back(material);
    }

    auto &attrib = reader.GetAttrib();
    std::vector<std::vector<Face>> matGroups(materials.size());
    for (const auto &shape: reader.GetShapes()) {
        tinyobj::mesh_t mesh = shape.mesh;
        int index_counter = 0;

        //face
        for (int i = 0; i < shape.mesh.num_face_vertices.size(); i++) {
            Face *face = new Face();
            int material = shape.mesh.material_ids[i];

            for (int j = 0; j < 3; j++) {
                tinyobj::index_t index = shape.mesh.indices[index_counter];

                face->vertices.push_back(attrib.vertices[index.vertex_index * 3]);
                face->vertices.push_back(attrib.vertices[(index.vertex_index * 3) + 1]);
                face->vertices.push_back(attrib.vertices[(index.vertex_index * 3) + 2]);

                if (index.normal_index >= 0) {
                    face->vertices.push_back(attrib.normals[3 * size_t(index.normal_index)]);
                    face->vertices.push_back(attrib.normals[3 * size_t(index.normal_index) + 1]);
                    face->vertices.push_back(attrib.normals[3 * size_t(index.normal_index) + 2]);
                } else {
                    face->vertices.push_back(0);
                    face->vertices.push_back(0);
                    face->vertices.push_back(0);
                }

                if (index.texcoord_index >= 0) {
                    face->vertices.push_back(attrib.texcoords[2 * size_t(index.texcoord_index)]);
                    face->vertices.push_back(attrib.texcoords[2 * size_t(index.texcoord_index) + 1]);
                } else {
                    face->vertices.push_back(0);
                    face->vertices.push_back(0);
                }

                index_counter++;
            }

            // material -1 gets set to default material at postition 0;
            matGroups[material + 1].push_back(*face);

        }
    }

    std::vector<RenderGroup*> renderGroups;

    for (int i = 0; i < matGroups.size(); i++) {
        RenderGroup *group = new RenderGroup(matGroups.at(i), materials.at(i));
        renderGroups.push_back(group);
    }

    matGroups.clear();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        if (wireFrame) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }


        program->setMatx4fv("projection",
                            glm::perspective(glm::radians(fov), (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1f,
                                             10000.0f));
        program->setMatx4fv("view", glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
        program->setMatx4fv("model", glm::mat4(1.0f));

        program->set3fv("cameraPos", cameraPos);
        program->setInt("mode", mode);


        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

        program->set3fv("light.position", lightPos);
        program->set3fv("light.ambient", ambientColor);
        program->set3fv("light.diffuse", diffuseColor);
        program->set3fv("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (const auto &group: renderGroups) {
            group->render(*program);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}