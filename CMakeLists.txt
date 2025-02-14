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
        src/Window.cpp
        src/Program.cpp
        src/Material.cpp
        src/Mesh.cpp
        src/Camera.cpp
        src/Loader.cpp
        src/Mesh.h
        src/LightingManager.cpp
        src/LightingManager.cpp
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

# Define the shaders source directory and destination directory
# Define the shaders source directory and destination directory
set(SHADERS_DIR ${CMAKE_SOURCE_DIR}/shaders)
set(SHADERS_DEST ${CMAKE_BINARY_DIR}/shaders)

# Add a custom post-build command to copy the shaders
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_DEST}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADERS_DIR} ${SHADERS_DEST}
        COMMENT "Copying shaders to build directory"
)