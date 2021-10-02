cmake_minimum_required(VERSION 3.16)
project(svr_cubeworld)

set(CMAKE_CXX_STANDARD 11)
set(ACE_ROOT "extern/ACE_wrappers/build/default")
set(TAO_ROOT "${ACE_ROOT}/TAO")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
  )
FetchContent_MakeAvailable(googletest)

set(ACE_LIB ${ACE_ROOT}/lib)
find_library(TAO_IORTable SHARED TAO_IORTable PATHS "${ACE_LIB}")
find_library(libTAO_PortableServer SHARED TAO_PortableServer PATHS "${ACE_LIB}")
find_library(libTAO_AnyTypeCode SHARED TAO_AnyTypeCode PATHS "${ACE_LIB}")
find_library(libTAO SHARED TAO PATHS "${ACE_LIB}")
find_library(libACE SHARED ACE PATHS "${ACE_LIB}")

message(STATUS "${ACE_LIB}")


add_executable(${PROJECT_NAME} main.cpp testC.cpp  test_i.cpp  testS.cpp ServerWorker.cpp svrAppl.cpp shader_s.cpp svrCubeWorld.cpp svrServer.cpp)
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

include_directories(${ACE_ROOT})
include_directories(${TAO_ROOT})

target_link_libraries(${PROJECT_NAME} OpenGL::GL glfw glm "${TAO_IORTable}" "${libTAO_PortableServer}" "${libTAO_AnyTypeCode}" "${libTAO}" "${libACE}")


enable_testing()

add_executable(
  ovr_test
  tests/ovr_test.cc
  svrCubeWorld.cpp
)
target_link_libraries(
  ovr_test
  gtest_main
)

add_executable(
  gl_test
  tests/gl_test.cc
)
target_link_libraries(
  gl_test
  gtest_main glfw glm
)

include(GoogleTest)
gtest_discover_tests(ovr_test gl_test)
