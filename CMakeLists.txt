cmake_minimum_required(VERSION 3.16)
project(svr_cubeworld)

include(ExternalProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-narrowing")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O1")

set(ACE_ROOT "extern/ACE_wrappers/build/default")
set(TAO_ROOT "${ACE_ROOT}/TAO")
set(ORBSVCS_ROOT "${TAO_ROOT}/orbsvcs")



set(OPENH264_ROOT "extern/openh264")

set(OPENCV_ROOT "extern/opencv")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
  )
FetchContent_MakeAvailable(googletest)

set(ACE_LIB ${ACE_ROOT}/lib)
set(OPENCV_LIB ${OPENCV_ROOT}/build/lib)



find_library(TAO_Svc_Utils SHARED TAO_Svc_Utils PATHS "${ACE_LIB}") 
find_library(TAO_IFR_Client SHARED TAO_IFR_Client PATHS "${ACE_LIB}")
find_library(TAO_DynamicInterface SHARED TAO_DynamicInterface PATHS "${ACE_LIB}")
find_library(TAO_Messaging SHARED TAO_Messaging PATHS "${ACE_LIB}")
find_library(TAO_PI SHARED TAO_PI PATHS "${ACE_LIB}")
find_library(TAO_CodecFactory SHARED TAO_CodecFactory PATHS "${ACE_LIB}")
find_library(TAO_Valuetype SHARED TAO_ValueType PATHS "${ACE_LIB}")



find_library(libTAO_CosNaming SHARED TAO_CosNaming PATHS "${ACE_LIB}")
find_library(libTAO_CosEvent_Skel SHARED TAO_CosEvent_Skel PATHS "${ACE_LIB}")
find_library(libTAO_CosEvent_Serv SHARED TAO_CosEvent_Serv PATHS "${ACE_LIB}")
find_library(libTAO_CosEvent SHARED TAO_CosEvent PATHS "${ACE_LIB}")
find_library(libTAO_IORTable SHARED TAO_IORTable PATHS "${ACE_LIB}")
find_library(libTAO_PortableServer SHARED TAO_PortableServer PATHS "${ACE_LIB}")
find_library(libTAO_AnyTypeCode SHARED TAO_AnyTypeCode PATHS "${ACE_LIB}")
find_library(libTAO SHARED TAO PATHS "${ACE_LIB}")
find_library(libACE SHARED ACE PATHS "${ACE_LIB}")
message(STATUS "${ACE_LIB}")


find_library(libopenh264 SHARED openh264 PATHS "${OPENH264_ROOT}")
find_library(libopencv_core SHARED opencv_core PATHS "${OPENCV_LIB}")
find_library(libopencv_imgcodecs SHARED opencv_imgcodecs PATHS "${OPENCV_LIB}")
find_library(libopencv_imgproc SHARED opencv_imgproc PATHS "${OPENCV_LIB}")

add_executable(${PROJECT_NAME} main.cpp testC.cpp  test_i.cpp  testS.cpp svr/svrWorker.cpp svr/svrAppl.cpp shader_s.cpp svr/svrCubeWorld.cpp svr/svrServer.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${ORBSVCS_ROOT})


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
include_directories(${LIBJPEG_ROOT})
include_directories(${OPENH264_ROOT})
include_directories(${OPENCV_ROOT})
include_directories(extern/opencv/modules/core/include/)
include_directories(${ORBSVCS_ROOT})

target_link_libraries(${PROJECT_NAME} OpenGL::GL glfw glm "${libopencv_imgproc}" "${libopencv_imgcodecs}" "${libopencv_core}" "${libopenh264}" "${libTAO_CosNaming}" "${libTAO_CosEvent}" "${libTAO_CosEvent_Serv}" "${libTAO_CosEvent_Skel}" "${libTAO_IORTable}" "${libTAO_PortableServer}" "${libTAO_AnyTypeCode}" "${libTAO}" "${libACE}")

add_executable(
  client
  client.cpp testC.cpp shader_s.cpp
  )
target_link_libraries(client OpenGL::GL glfw glm "${libopencv_imgproc}" "${libopencv_imgcodecs}" "${libopencv_core}" "${libopenh264}" "${libturbojpeg}" "${libTAO_IORTable}" "${libTAO_PortableServer}" "${libTAO_AnyTypeCode}" "${libTAO}" "${libACE}")

add_executable(
  Service
  Service.cpp
)

target_link_libraries(Service "${libTAO_CosEvent_Serv}" "${libTAO_CosNaming}" "${TAO_Svc_Utils}" "${TAO_IFR_Client}" "${TAO_DynamicInterface}" "${TAO_Messaging}" "${TAO_PI}" "${TAO_CodecFactory}" "${TAO_ValueType}" "${libTAO_CosEvent_Skel}" "${libTAO_PortableServer}" "${libTAO_CosEvent}" "${libTAO_AnyTypeCode}" "${libTAO}" "${libACE}")

enable_testing()

add_executable(
  ovr_test
  tests/ovr_test.cc
  svr/svrCubeWorld.cpp
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
