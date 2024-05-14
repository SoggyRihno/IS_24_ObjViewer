cmake_minimum_required(VERSION 3.27)
project(OpenGLTest)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(${CMAKE_SOURCE_DIR}/dep/glfw-3.3.9)

add_executable(OpenGLTest
        src/main.cpp
        src/glad.c
        src/Program.cpp
        src/Texture.cpp
        src/Material.cpp
        src/RenderGroup.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)


