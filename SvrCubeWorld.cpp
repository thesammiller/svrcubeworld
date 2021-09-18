#include "SvrCubeWorld.h"

#include <GLFW/glfw3.h>
#include "vertex_data.h"


SvrCubeWorld::SvrCubeWorld()
    : svrAppl(),  //0, 0, CPU_LEVEL, GPU_LEVEL, true /* useMultiView */),
    Random(2)
       {
    std::cout << "Created Server Cube World Object" << std::endl;    
    // VrApi_Helpers.h
    CenterEyeViewMatrix = ovrMatrix4f_CreateIdentity();
}

//SvrCubeWorld::SvrCubeWorld() {
//    
//}

bool SvrCubeWorld::AppInit() {
    std::cout << "SvrCubeWorld -- Initialize App" << std::endl;

    //Ignore JNI
    //Ignore FileSys
    //Ignore Locle
    //Ignore SoundEffectPlayer
    //Ignore GuiSys

    Shader shaders = Shader("vertexShader.vs", "fragmentShader.fs");

    const char* vShader = shaders.vShaderCode;
    const char* fShader = shaders.fShaderCode;

    // Uses the GlProgram class to compile shaders
    Program = GlProgram::Build(vShader, fShader, nullptr, 0);
    VertexTransformAttribute = glGetAttribLocation(Program.Program, "VertexTransform");

    // Create the cube.
    VertexAttribs attribs;
    attribs.position.resize(8);
    attribs.color.resize(8);
    for (int i = 0; i < 8; i++) {
        attribs.position[i] = cubeVertices.positions[i];
        attribs.color[i] = cubeVertices.colors[i];
    }

    std::vector<TriangleIndex> indices;
    indices.resize(36);
    for (int i = 0; i < 36; i++) {
        indices[i] = cubeIndices[i];
    }

    //GlGeometry.cpp
    Cube.Create(attribs, indices);


    return true;
}

// Returns a random float in the range [0, 1].
float SvrCubeWorld::RandomFloat() {
    Random = 1664525L * Random + 1013904223L;
    unsigned int rf = 0x3F800000 | (Random & 0x007FFFFF);
    return (*(float*)&rf) - 1.0f;
}