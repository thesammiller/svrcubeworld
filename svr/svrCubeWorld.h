


#include "../ovr/VrApi_Helpers.h"
#include "../ovr/OVR_Math.h"

using OVR::Vector3f;
using OVR::Vector4f;

// setup Cube
struct ovrCubeVertices {
    Vector3f positions[8];
    Vector4f colors[8];
};

static ovrCubeVertices cubeVertices = {
    // positions
    {
        Vector3f(-1.0f, +1.0f, -1.0f),
        Vector3f(+1.0f, +1.0f, -1.0f),
        Vector3f(+1.0f, +1.0f, +1.0f),
        Vector3f(-1.0f, +1.0f, +1.0f), // top
        Vector3f(-1.0f, -1.0f, -1.0f),
        Vector3f(-1.0f, -1.0f, +1.0f),
        Vector3f(+1.0f, -1.0f, +1.0f),
        Vector3f(+1.0f, -1.0f, -1.0f) // bottom
    },
    // colors
    {Vector4f(1.0f, 0.0f, 1.0f, 1.0f),
     Vector4f(0.0f, 1.0f, 0.0f, 1.0f),
     Vector4f(0.0f, 0.0f, 1.0f, 1.0f),
     Vector4f(1.0f, 0.0f, 0.0f, 1.0f),
     Vector4f(0.0f, 0.0f, 1.0f, 1.0f),
     Vector4f(0.0f, 1.0f, 0.0f, 1.0f),
     Vector4f(1.0f, 0.0f, 1.0f, 1.0f),
     Vector4f(1.0f, 0.0f, 0.0f, 1.0f)},
};



static const unsigned short cubeIndices[36] = {
    0, 2, 1, 2, 0, 3, // top
    4, 6, 5, 6, 4, 7, // bottom
    2, 6, 7, 7, 1, 2, // right
    0, 4, 5, 5, 3, 0, // left
    3, 5, 6, 6, 2, 3, // front
    0, 1, 7, 7, 4, 0 // back
};


float RandomFloat();

void generate_random_rotations(ovrVector3f *Rotations, int NUM_ROTATIONS);
void generate_random_rotations(ovrVector3f *Rotations, int NUM_ROTATIONS);
void generate_random_locations(ovrVector3f *CubePositions, int *CubeRotations, int NUM_INSTANCES, ovrVector3f *Rotations, int NUM_ROTATIONS);
void update_cube_rotations(float startTime, float currentTime, ovrVector3f *Rotations, 
                        int NUM_ROTATIONS, ovrMatrix4f *cubeTransforms, int NUM_INSTANCES, int *CubeRotations, ovrVector3f *CubePositions);