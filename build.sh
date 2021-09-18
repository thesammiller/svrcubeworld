#!/bin/sh
g++ main.cpp  -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -DGL_GLEXT_PROTOTYPES -o gl_main -fpermissive

