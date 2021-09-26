cmake_minimum_required(VERSION 3.13.4)

project(SVR_CubeWorld)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC m glfw GL X11 pthread Xrandr Xi)

#default: g++  main.cpp -g  -lm -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -DGL_GLEXT_PROTOTYPES -o out

