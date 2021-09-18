#include "SvrCubeWorld.h"

#include <GLFW/glfw3.h>

#include "Egl.h"

#include "vertex_data.h"



double GetTimeInSeconds() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (now.tv_sec * 1e9 + now.tv_nsec) * 0.000000001;
}



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

    // Setup the instance transform attributes.
    GL(glBindVertexArray(Cube.vertexArrayObject));
    GL(glGenBuffers(1, &InstanceTransformBuffer));
    GL(glBindBuffer(GL_ARRAY_BUFFER, InstanceTransformBuffer));
    GL(glBufferData(
        GL_ARRAY_BUFFER, NUM_INSTANCES * 4 * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW));
    for (int i = 0; i < 4; i++) {
        GL(glEnableVertexAttribArray(VertexTransformAttribute + i));
        GL(glVertexAttribPointer(
            VertexTransformAttribute + i,
            4,
            GL_FLOAT,
            false,
            4 * 4 * sizeof(float),
            (void*)(i * 4 * sizeof(float))));
        GL(glVertexAttribDivisor(VertexTransformAttribute + i, 1));
    }
    GL(glBindVertexArray(0));

    // Setup random rotations.
    for (int i = 0; i < NUM_ROTATIONS; i++) {
        Rotations[i].x = RandomFloat();
        Rotations[i].y = RandomFloat();
        Rotations[i].z = RandomFloat();
    }

    // Setup random cube positions and rotations.
    for (int i = 0; i < NUM_INSTANCES; i++) {
        volatile float rx, ry, rz;
        for (;;) {
            rx = (RandomFloat() - 0.5f) * (50.0f + static_cast<float>(sqrt(NUM_INSTANCES)));
            ry = (RandomFloat() - 0.5f) * (50.0f + static_cast<float>(sqrt(NUM_INSTANCES)));
            rz = (RandomFloat() - 0.5f) * (50.0f + static_cast<float>(sqrt(NUM_INSTANCES)));

            // If too close to 0,0,0
            if (fabsf(rx) < 4.0f && fabsf(ry) < 4.0f && fabsf(rz) < 4.0f) {
                continue;
            }

            // Test for overlap with any of the existing cubes.
            bool overlap = false;
            for (int j = 0; j < i; j++) {
                if (fabsf(rx - CubePositions[j].x) < 4.0f &&
                    fabsf(ry - CubePositions[j].y) < 4.0f &&
                    fabsf(rz - CubePositions[j].z) < 4.0f) {
                    overlap = true;
                    break;
                }
            }

            if (!overlap) {
                break;
            }
        }

        rx *= 0.1f;
        ry *= 0.1f;
        rz *= 0.1f;

        // Insert into list sorted based on distance.
        int insert = 0;
        const float distSqr = rx * rx + ry * ry + rz * rz;
        for (int j = i; j > 0; j--) {
            const ovrVector3f* otherPos = &CubePositions[j - 1];
            const float otherDistSqr =
                otherPos->x * otherPos->x + otherPos->y * otherPos->y + otherPos->z * otherPos->z;
            if (distSqr > otherDistSqr) {
                insert = j;
                break;
            }
            CubePositions[j] = CubePositions[j - 1];
            CubeRotations[j] = CubeRotations[j - 1];
        }

        CubePositions[insert].x = rx;
        CubePositions[insert].y = ry;
        CubePositions[insert].z = rz;

        CubeRotations[insert] = (int)(RandomFloat() * (NUM_ROTATIONS - 0.1f));
    }

    // Create SurfaceDef
    SurfaceDef.surfaceName = "VrCubeWorld Framework";
    SurfaceDef.graphicsCommand.Program = Program;
    SurfaceDef.graphicsCommand.GpuState.blendEnable = ovrGpuState::BLEND_ENABLE;
    SurfaceDef.graphicsCommand.GpuState.cullEnable = true;
    SurfaceDef.graphicsCommand.GpuState.depthEnable = true;
    SurfaceDef.geo = Cube;
    SurfaceDef.numInstances = NUM_INSTANCES;

    SurfaceRender.Init();

    startTime = GetTimeInSeconds();




    return true;
}

// Returns a random float in the range [0, 1].
float SvrCubeWorld::RandomFloat() {
    Random = 1664525L * Random + 1013904223L;
    unsigned int rf = 0x3F800000 | (Random & 0x007FFFFF);
    return (*(float*)&rf) - 1.0f;
}