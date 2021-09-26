cmake_minimum_required(VERSION 3.16)
project(svr_cubeworld)

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
  )
FetchContent_MakeAvailable(googletest)

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


enable_testing()

add_executable(
  ovr_test
  tests/ovr_test.cc
)
target_link_libraries(
  ovr_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(ovr_test)
