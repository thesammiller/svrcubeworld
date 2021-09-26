


#include "VrApi_Helpers.h"
#include "OVR_Math.h"

using OVR::Vector3f;
using OVR::Vector4f;

unsigned int Random;  

void generate_random_rotations(ovrVector3f *Rotations, int NUM_ROTATIONS);

float RandomFloat() {
    Random = 1664525L * Random + 1013904223L;
    unsigned int rf = 0x3F800000 | (Random & 0x007FFFFF);
    return (*(float*)&rf) - 1.0f;
}

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




void generate_random_rotations(ovrVector3f *Rotations, int NUM_ROTATIONS) {
    for (int i = 0; i < NUM_ROTATIONS; i++) {
            Rotations[i].x = RandomFloat();
            Rotations[i].y = RandomFloat();
            Rotations[i].z = RandomFloat();
        }
}

void generate_random_locations(ovrVector3f *CubePositions, int *CubeRotations, int NUM_INSTANCES, ovrVector3f *Rotations, int NUM_ROTATIONS) {

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
                if (fabsf(rx - CubePositions[j].x) < 4.0f && //4
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

        //rx *= 0.1f;
        //ry *= 0.1f;
        //rz *= 0.1f;

        // Insert into list sorted based on distance.
        int insert = 0;
        const float distSqr = rx * rx + ry * ry + rz * rz;
        for (int j = i; j > 0; j--) {
            ovrVector3f* otherPos = &CubePositions[j - 1];
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
}


void update_cube_rotations(float startTime, float currentTime, ovrVector3f *Rotations, 
                        int NUM_ROTATIONS, ovrMatrix4f *cubeTransforms, int NUM_INSTANCES, int *CubeRotations, ovrVector3f *CubePositions) {
    ovrVector3f currentRotation;
    currentRotation.x = (float)(currentTime - startTime);
    currentRotation.y = (float)(currentTime - startTime);
    currentRotation.z = (float)(currentTime - startTime);
        
    ovrMatrix4f rotationMatrices[NUM_ROTATIONS];
    for (int i = 0; i < NUM_ROTATIONS; i++) {
        rotationMatrices[i] = ovrMatrix4f_CreateRotation(
            Rotations[i].x * currentRotation.x,
            Rotations[i].y * currentRotation.y,
            Rotations[i].z * currentRotation.z);
        }
    
    for (int i = 0; i < NUM_INSTANCES; i++) {
            const int index = CubeRotations[i];

            // Write in order in case the mapped buffer lives on write-combined memory.
            cubeTransforms[i].M[0][0] = rotationMatrices[index].M[0][0];
            cubeTransforms[i].M[0][1] = rotationMatrices[index].M[0][1];
            cubeTransforms[i].M[0][2] = rotationMatrices[index].M[0][2];
            cubeTransforms[i].M[0][3] = rotationMatrices[index].M[0][3];

            cubeTransforms[i].M[1][0] = rotationMatrices[index].M[1][0];
            cubeTransforms[i].M[1][1] = rotationMatrices[index].M[1][1];
            cubeTransforms[i].M[1][2] = rotationMatrices[index].M[1][2];
            cubeTransforms[i].M[1][3] = rotationMatrices[index].M[1][3];

            cubeTransforms[i].M[2][0] = rotationMatrices[index].M[2][0];
            cubeTransforms[i].M[2][1] = rotationMatrices[index].M[2][1];
            cubeTransforms[i].M[2][2] = rotationMatrices[index].M[2][2];
            cubeTransforms[i].M[2][3] = rotationMatrices[index].M[2][3];

            cubeTransforms[i].M[3][0] = CubePositions[i].x;
            cubeTransforms[i].M[3][1] = CubePositions[i].y;
            cubeTransforms[i].M[3][2] = CubePositions[i].z;
            cubeTransforms[i].M[3][3] = 1.0f;
        }

}