cmake_minimum_required(VERSION 3.10)
project(sample)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_MODULE_PATH /usr/local/lib/cmake /usr/local/lib/x86_64-linux-gnu/cmake)
set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw)

find_package (PkgConfig REQUIRED)
find_package (OpenGL REQUIRED)
find_package (glfw3 REQUIRED)
find_package (glm REQUIRED)

set(SOURCE_FILES main.cpp)
add_executable(sample ${SOURCE_FILES})
target_link_libraries (sample ${OPENGL_LIBRARIES} ${GLFW3_LIBRARY}
        Xxf86vm pthread Xrandr Xinerama Xi Xcursor)

      # glfw
add_subdirectory(glfw/src EXCLUDE_FROM_ALL)
target_link_libraries(sample PRIVATE glfw)


# glm
add_subdirectory(glm/glm EXCLUDE_FROM_ALL)
target_link_libraries(sample PRIVATE glm)
