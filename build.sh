#!/bin/sh
g++ main.cpp SvrCubeWorld.cpp Appl.cpp SurfaceRender.cpp shader_h.cpp GlProgram.cpp -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -DGL_GLEXT_PROTOTYPES -o gl_main -fpermissive

