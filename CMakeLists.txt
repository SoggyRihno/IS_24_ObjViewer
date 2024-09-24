cmake_minimum_required(VERSION 3.27)
project(ObjViewer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# FetchContent for downloading dependencies
include(FetchContent)

# Fetch GLFW
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        master
)
FetchContent_MakeAvailable(glfw)

# Fetch GLAD (OpenGL loader)
FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG        master
)
FetchContent_MakeAvailable(glad)

# Fetch GLM (math library)
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG        master
)
FetchContent_MakeAvailable(glm)

# Fetch TinyOBJLoader (OBJ file loader)
FetchContent_Declare(
        tinyobjloader
        GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
        GIT_TAG        release  # the master is named release
)
FetchContent_MakeAvailable(tinyobjloader)


# Fetch stb_image (image loading)
FetchContent_Declare(
        stb_image
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG        master
)
FetchContent_MakeAvailable(stb_image)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${stb_image_SOURCE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/src)
add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Window.h
        src/Window.cpp
        src/Program.cpp
        src/Loader.h
        src/Loader.h
        src/Loader.cpp
        src/Material.cpp
        src/Material.h
        src/Mesh.h
        src/Mesh.cpp
        src/Camera.cpp
)

target_link_libraries(${PROJECT_NAME}
        glad
        glfw
        glm
        tinyobjloader
        stb_image
)

# Ensure OpenGL is linked
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME}  OpenGL::GL)