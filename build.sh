#!/bin/sh
g++ svr_cube_world.cpp  -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -DGL_GLEXT_PROTOTYPES -o gl_main -fpermissive

