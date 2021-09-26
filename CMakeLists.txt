cmake_minimum_required(VERSION 3.13.4)

project(SVR_CubeWorld)

add_executable(${PROJECT_NAME} main.cpp)

find_package(glfw3 3.3 REQUIRED)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC m glfw X11 pthread Xrandr Xi OpenGL::GL)





#default: g++  main.cpp -g  -lm -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -DGL_GLEXT_PROTOTYPES -o out

