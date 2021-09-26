cmake_minimum_required(VERSION 3.16)
project(svr_cubeworld)

set(CMAKE_CXX_STANDARD 11)


add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

# OpenGL
find_package(OpenGL REQUIRED)
message(STATUS "OpenGL included at ${OPENGL_INCLUDE_DIR}")

# GLM
add_subdirectory(extern/glm)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

# GLFW
add_subdirectory(extern/glfw)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

target_link_libraries(${PROJECT_NAME} OpenGL::GL glfw glm)

